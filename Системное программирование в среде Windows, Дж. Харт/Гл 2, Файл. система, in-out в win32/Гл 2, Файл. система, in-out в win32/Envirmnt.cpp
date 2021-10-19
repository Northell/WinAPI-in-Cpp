#include "Envirmnt.h"

BOOL Envirmnt::printStrings(HANDLE hOut, LPCTSTR pMsg)
{
    DWORD MsgLen, Count;

    va_list pMsgList;   //Строка текущего сообщения
    
    //Начать обработку сообщений
    va_start(pMsgList, hOut);   
    while ((pMsg = va_arg(pMsgList, LPCTSTR)) != NULL)
    {
        MsgLen = wcslen(pMsg);
        /*
        Функция WriteConsole может применяться только с дескриптором буффера
        экрана консоли. 
        */
        if(!WriteConsole(hOut, pMsg, MsgLen, &Count, NULL)
           /*
           Функция WriteFile вызывается только в случае неудачного завершения функции 
           WriteConsole
           */
            && !WriteFile(hOut, pMsg, MsgLen * sizeof(TCHAR), &Count, NULL))
        {
            return FALSE;
        }   
    }
    va_end(pMsgList);
    return TRUE;
}

BOOL Envirmnt::printMsg(HANDLE hOut, LPCTSTR pMsg)
{
    return printStrings(hOut, pMsg);
}

BOOL Envirmnt::ConsolePromt(LPCTSTR pPromtMsg, LPTSTR pResponse, DWORD MaxTchar, BOOL Echo)
{
    HANDLE hStdIn, hStdOut;

    hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD TcharIn, EchoFlag;
    BOOL Success;
    hStdIn = CreateFile(L"CONINS",
        GENERIC_READ | GENERIC_WRITE, 0,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    EchoFlag = Echo ? ENABLE_ECHO_INPUT : 0;
    Success =
           SetConsoleMode(hStdIn, ENABLE_LINE_INPUT | EchoFlag | ENABLE_PROCESSED_INPUT)
        && SetConsoleMode(hStdOut, ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_PROCESSED_OUTPUT)
        && printStrings(hStdOut, pPromtMsg)
        && ReadConsole(hStdIn, pResponse, MaxTchar, &TcharIn, NULL);

    if (Success)pResponse[TcharIn - 2] = '\0';

    CloseHandle(hStdIn);
    CloseHandle(hStdOut);
    return 0;
}
