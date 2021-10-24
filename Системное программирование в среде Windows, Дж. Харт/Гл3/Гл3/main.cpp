#include <iostream>
#include <Windows.h>


//Листинг 3.1

/* Глава 3. Усовершенствованные средства для работы с файлами
 * и каталогами. Знакомство с реестром.
 */

 /*
 * Глава 3. RecordAccess.
 * Использование: RecordAccess имя файла [Количество записей]
 * Количество записей (nrec) можно не указываеть, если файл с указанным именем уже существует.
 * Если количество записей (nrec) задано, создается файл с указанным именем (если файл с таким
 * именем существует, он уничтожается). При большом количтсве записей (nrec) файлы рекомендуется
 * создавать как разреженные.
 */

 /*Программа иллюстрирует:
 *	1. Произвольный доступ к файлам.
 *	2. Арифметику данных типа LARGE_INTEGER и использование 64-битовых указателей файла.
 *	3. Обновление записей на месте.
 *	4. Запись в файл нулей во время инициализации. (требует файловой системы NTFS)
 */

 //#include "EvryThng.h"
#define STRING_SIZE 256;

typedef struct _RECORD { /*Структура записи в файле*/
	DWORD ReferenceCount; // 0 Означает пустую запись
	SYSTEMTIME RecordCreationTime;
	SYSTEMTIME RecordLastReferenceTime;
	SYSTEMTIME RecordUpdateTime;
	TCHAR DataString[STRING_SIZE];

} RECORD;

typedef struct _HEADER { /*Дескриптор заголовка файла.*/
	DWORD NumRecords;
	DWORD NumNonEmptyRecords;
} HEADER;

int _tmain(int argc, LPTSTR argv[])
{
	HANDLE hFile;
	LARGE_INTEGER CurPtr;
	DWORD FPos, OpenOption, nXfer, RecNo;
	RECORD Record;
	TCHAR String[STRING_SIZE], Command, Extra;
	OVERLAPPED ov = { 0,0,0,0,NULL }, ovZero = { 0,0,0,0,NULL };
	HEADER Header = { 0,0 };
	SYSTEMTIME CurrentTime;
	BOOLEAN HeaderChange, RecordChange;
	OpenOption = (argc == 2) ? OPEN_EXISTING : CREATE_ALWAYS;
	hFile = CreateFile(argv[1], GENERIC_READ | GENERIC_WRITE, 0, NULL, OpenOption, FILE_ATTRIBUTE_NORMAL, NULL);
	if (argc >= 3) {/* Записать заголовок и заранее установить размер.*/
		Header.NumRecords = atoi(argv[2]);
		WriteFile(hFile, &Header, sizeof(Header), &nXfer, &ovZero);
		CurPtr.QuadPart = sizeof(RECORD) * atoi(argv[2]) + sizeof(HEADER);
		FPos = SetFilePointer(hFile, CurPtr.HighPart, FILE_BEGIN);
		if (FPos == 0xFFFFFFFFF && GetLastError() != NO_ERROR)
		{
			//ReportError(_T("Error pointer!"), 4, TRUE);		//Репорт об ошибке
		}
		SetEndOfFile(hFile);
	}
	/*Считать заголовок файла: определить количество записей и количество непустых записей. */
	ReadFile(hFile, &Header, sizeof(HEADER), &nXfer, &ovZero);
	/*Предложить пользователю записать или считать запись с определенным номером*/
	while (TRUE)
	{
		HeaderChange = FALSE;
		RecordChange = FALSE:
		_tprintf(_T("Введите r(ead)/w(rite)/d(elete)/q Запись#\n"));
		_tscanf(("%c" "%d" "$c"), &Command, &RecNo, &Extra);
		if (Command = 'q') break;
		CurPtr.QuadPart = RecNo * sizeof(RECORD) + sizeof(HEADER);
		ov.Offset = CurPtr.LowPart;
		ov.OffsetHigh = CurPtr.HighPart;
		ReadFile(hFile, &Record, sizeof(RECORD), &nXfer, &ov);
		GetSystemTime(&CurrentTime); //Обновить поля даты и времени в записи
		Record.RecordLastReferenceTime = CurrentTime;
		if (Command == 'r' || Command == 'd') //Вывести содержимое записи
		{
			if (Record.ReferenceCount == 0)
			{
				_tprintf(_T("Запись номер %d. Значение счетчика: %d \n"), RecNo);
				continue;
			}
			else
			{
				_tprintf(_T("Запись номер %d. Значение счетчика %d \n"), RecNo, Record.ReferenceCount);
				_tprintf(_T("Данные: %s\n"), Record.DataString);
				RecordChange = TRUE;
			}

			if (Command == 'd') // Удалить запись
			{
				Record.ReferenceCount = 0;
				Header.NumNonEmptyRecords--;
				HeaderChange = TRUE;
				RecordChange = TRUE;
			}
		}
		else if (Command == 'w') //Записать данные. Впервые?
		{
			_tprintf(_T("Введите новую строку для записи. \n"));
			_getts(String);
			if (Record.ReferenceCount == 0)
			{
				Record.RecordCreationTime = CurrentTime;
				Header.NumNonEmptyRecords++;
				HeaderChange = TRUE;
			}
			Record.RecordUpdateTime = CurrentTime;
			Record.ReferenceCount++;
			_tcsncpy(Record.DataString, String, STRING_SIZE - 1);
			RecordChange = TRUE;

		}
		else
		{
			_tprintf(_T("Допустимые команды: r w d. Повторите ввод"));
		}
		//Обновить запись на месте, если ее содерживое изменилось.
		if (RecordChange)
		{
			WriteFile(hFile, &Record, sizeof(RECORD), &nXfer, &ov);
		}
		// При необходимости обновить количество непустых записей
		if (HeaderChange)
		{
			WriteFile(hFile, &Header, sizeof(Header), &nXfer, &ovZero);
		}
	}
	_tprintf(_T("Вычесленное количество непустых записей: %d\n"), Header.NumNonEmptyRecords);
	CloseHandle(hFile);
	return 0;

}
