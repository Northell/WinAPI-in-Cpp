#include <windows.h>
#include <iostream>

/*
* ��� 129, ��4.1 ������� ��������� ����������
*/
//bool createTempFile()
//{
//	std::string TempFile;	//���� � �����
//	HANDLE hFile;
//	GetTempFileName(TempFile, ...);
//	while (...) __try {
//		hFile = CreateFile(TempFile, ..., OPEN_ALWAYS, ...);
//		SetFilePointer(hFile, 0, NULL, FILE_END);
//		...
//			WriteFile(hFile, ...);
//		i = *p;	//� ���� ����� ��������� �������� ������������� ����������
//		CloseHandle(hFile);
//
//	}
//	__except (EXEPTION_EXECUTE_HANDLER)
//	{
//		CloseHandle(hFile);
//		DeleteFile(TempFile);
//		/*������� � ���������� ��������� �������� �����*/
//	}
//	/*���� ���������� ���������� ����� ����������� ���������� �����.*/
//}

/*��� 137, ReportException
	���������� ������� ReportError ��� ��������� ������������ ����������� ���� ����������
	������ ����������� ���������� ��������.
*/
VOID ReportException(LPCSTR UserMessage, DWORD ExceptionCode)
{
	ReportError(UserMessage, 0, TRUE);	
	//���� ������ �����������, ������������� ����������
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