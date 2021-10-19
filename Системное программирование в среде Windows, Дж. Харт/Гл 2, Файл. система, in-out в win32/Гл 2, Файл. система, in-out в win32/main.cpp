#include <Windows.h>
#include "Envirmnt.h"
/// <summary>
/// Ћистинг 2.1: ¬ывод в консоль заданное сообщение 
///	 и возвращение ответа пользовател€ на него
/// </summary>
/// <returns></returns>
int main()
{
	Envirmnt env;
	HANDLE hndle = GetStdHandle(STD_OUTPUT_HANDLE);
	char response[10];
	bool nReulst = env.ConsolePromt(L"Pipa?", (LPTSTR)response, sizeof("Pipa"), NULL);
		//CloseHandle(hndle);
	return 0;
}