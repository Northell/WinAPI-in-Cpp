#pragma once
#include <Windows.h>
#include <stdarg.h>
class Envirmnt
{
public: 
	BOOL printStrings(HANDLE hOut, LPCTSTR pMsg);			//������ ��������� � ������ ������ �������
	BOOL printMsg(HANDLE hOut, LPCTSTR pMsg);				//������ ��� ������ ���������� ���������
	BOOL ConsolePromt(LPCTSTR pPromtMsg, LPTSTR pResponse,	//������� �� ������� ��������� ��� ������������ � �������� �����
		DWORD MaxTchar, BOOL Echo); 
};

