#pragma once
#include <Windows.h>
#include <stdarg.h>
class Envirmnt
{
public: 
	BOOL printStrings(HANDLE hOut, LPCTSTR pMsg);			//Запись сообщений в буффер экрана консоли
	BOOL printMsg(HANDLE hOut, LPCTSTR pMsg);				//Версия для вывода одиночного сообщения
	BOOL ConsolePromt(LPCTSTR pPromtMsg, LPTSTR pResponse,	//Вывести на консоль подсказку для пользователя и получить ответ
		DWORD MaxTchar, BOOL Echo); 
};

