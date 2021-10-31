#include <windows.h>
#include <iostream>

/*
* стр 129, гл4.1 Обычная обработка исключений
*/
//bool createTempFile()
//{
//	std::string TempFile;	//Путь к файлу
//	HANDLE hFile;
//	GetTempFileName(TempFile, ...);
//	while (...) __try {
//		hFile = CreateFile(TempFile, ..., OPEN_ALWAYS, ...);
//		SetFilePointer(hFile, 0, NULL, FILE_END);
//		...
//			WriteFile(hFile, ...);
//		i = *p;	//В этом месте программы возможно возникновение исключения
//		CloseHandle(hFile);
//
//	}
//	__except (EXEPTION_EXECUTE_HANDLER)
//	{
//		CloseHandle(hFile);
//		DeleteFile(TempFile);
//		/*Переход к выполнению очередной итерации цикла*/
//	}
//	/*Сюда пережается управление после нормального завершения цикла.*/
//}

/*Стр 137, ReportException
	Расширение функции ReportError для генерации формируемого приложением кода исключения
	вместо прекращения выполнения процесса.
*/
VOID ReportException(LPCSTR UserMessage, DWORD ExceptionCode)
{
	ReportError(UserMessage, 0, TRUE);	
	//Если ошибка критическая, сгенерировать исключение
	if (ExceptionCode != 0)
	{
		RaiseException((0xFFFFFFFF & ExceptionCode) | 0xE0000000, 0, 0, NULL);
	}
	return;
}
int main()
{
	return 0;
}