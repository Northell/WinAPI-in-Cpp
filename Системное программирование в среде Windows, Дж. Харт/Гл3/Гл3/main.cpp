#include <iostream>
#include <Windows.h>


//������� 3.1

/* ����� 3. ������������������� �������� ��� ������ � �������
 * � ����������. ���������� � ��������.
 */

 /*
 * ����� 3. RecordAccess.
 * �������������: RecordAccess ��� ����� [���������� �������]
 * ���������� ������� (nrec) ����� �� ����������, ���� ���� � ��������� ������ ��� ����������.
 * ���� ���������� ������� (nrec) ������, ��������� ���� � ��������� ������ (���� ���� � �����
 * ������ ����������, �� ������������). ��� ������� ��������� ������� (nrec) ����� �������������
 * ��������� ��� �����������.
 */

 /*��������� ������������:
 *	1. ������������ ������ � ������.
 *	2. ���������� ������ ���� LARGE_INTEGER � ������������� 64-������� ���������� �����.
 *	3. ���������� ������� �� �����.
 *	4. ������ � ���� ����� �� ����� �������������. (������� �������� ������� NTFS)
 */

 //#include "EvryThng.h"
#define STRING_SIZE 256;

typedef struct _RECORD { /*��������� ������ � �����*/
	DWORD ReferenceCount; // 0 �������� ������ ������
	SYSTEMTIME RecordCreationTime;
	SYSTEMTIME RecordLastReferenceTime;
	SYSTEMTIME RecordUpdateTime;
	TCHAR DataString[STRING_SIZE];

} RECORD;

typedef struct _HEADER { /*���������� ��������� �����.*/
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
	if (argc >= 3) {/* �������� ��������� � ������� ���������� ������.*/
		Header.NumRecords = atoi(argv[2]);
		WriteFile(hFile, &Header, sizeof(Header), &nXfer, &ovZero);
		CurPtr.QuadPart = sizeof(RECORD) * atoi(argv[2]) + sizeof(HEADER);
		FPos = SetFilePointer(hFile, CurPtr.HighPart, FILE_BEGIN);
		if (FPos == 0xFFFFFFFFF && GetLastError() != NO_ERROR)
		{
			//ReportError(_T("Error pointer!"), 4, TRUE);		//������ �� ������
		}
		SetEndOfFile(hFile);
	}
	/*������� ��������� �����: ���������� ���������� ������� � ���������� �������� �������. */
	ReadFile(hFile, &Header, sizeof(HEADER), &nXfer, &ovZero);
	/*���������� ������������ �������� ��� ������� ������ � ������������ �������*/
	while (TRUE)
	{
		HeaderChange = FALSE;
		RecordChange = FALSE:
		_tprintf(_T("������� r(ead)/w(rite)/d(elete)/q ������#\n"));
		_tscanf(("%c" "%d" "$c"), &Command, &RecNo, &Extra);
		if (Command = 'q') break;
		CurPtr.QuadPart = RecNo * sizeof(RECORD) + sizeof(HEADER);
		ov.Offset = CurPtr.LowPart;
		ov.OffsetHigh = CurPtr.HighPart;
		ReadFile(hFile, &Record, sizeof(RECORD), &nXfer, &ov);
		GetSystemTime(&CurrentTime); //�������� ���� ���� � ������� � ������
		Record.RecordLastReferenceTime = CurrentTime;
		if (Command == 'r' || Command == 'd') //������� ���������� ������
		{
			if (Record.ReferenceCount == 0)
			{
				_tprintf(_T("������ ����� %d. �������� ��������: %d \n"), RecNo);
				continue;
			}
			else
			{
				_tprintf(_T("������ ����� %d. �������� �������� %d \n"), RecNo, Record.ReferenceCount);
				_tprintf(_T("������: %s\n"), Record.DataString);
				RecordChange = TRUE;
			}

			if (Command == 'd') // ������� ������
			{
				Record.ReferenceCount = 0;
				Header.NumNonEmptyRecords--;
				HeaderChange = TRUE;
				RecordChange = TRUE;
			}
		}
		else if (Command == 'w') //�������� ������. �������?
		{
			_tprintf(_T("������� ����� ������ ��� ������. \n"));
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
			_tprintf(_T("���������� �������: r w d. ��������� ����"));
		}
		//�������� ������ �� �����, ���� �� ���������� ����������.
		if (RecordChange)
		{
			WriteFile(hFile, &Record, sizeof(RECORD), &nXfer, &ov);
		}
		// ��� ������������� �������� ���������� �������� �������
		if (HeaderChange)
		{
			WriteFile(hFile, &Header, sizeof(Header), &nXfer, &ovZero);
		}
	}
	_tprintf(_T("����������� ���������� �������� �������: %d\n"), Header.NumNonEmptyRecords);
	CloseHandle(hFile);
	return 0;

}
