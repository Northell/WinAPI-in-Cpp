#include <windows.h>;

int CALLBACK wWinMain(HINSTANCE hinstance,// Указатель на начало испольняемого модуля
	HINSTANCE,//Актуален только для 16 битных приложений
	PWSTR szCadLine,// Pointer to white string, просто аргументы, которые передаются программе
	int nCmdShow)	// Отвечает за вид окна, свернутое, развернутое итд
	{
	MSG msg{}; // переменная для пересылки сообщений между объектами WinApi
	HWND hwnd{}; //тут хранится инфа о нашшем окне
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) }; //Отвечает за характеристики окна
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;			//Отвечают за выделение памяти в классе нашего окна

	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));			//цвет окраски окна
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);			//Курсор
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);			//Иконка
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);		//маленькая иконка для отображения в заголовке окна
	wc.hInstance = hinstance;								//
	wc.lpfnWndProc/*Это процедура(Указатель) отвечает за обработку сообщений*/ = [](HWND hWND,/*Дескриптор окна, которому приходит сообщение*/ 
																		UINT uMsg,/*Код сообщения*/ 
																		WPARAM wParam, LPARAM lParam/*В wParam и lParam необходимая инфа для сообщения*/)->LRESULT
	{
		switch (uMsg)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(EXIT_SUCCESS);
		}
		
			return 0;
		
		}
		return DefWindowProc(hWND, uMsg, wParam, lParam); //Вызов, если сообщение никак не обрабатывается
	};
	wc.lpszClassName = L"MyAppClass";
	wc.lpszMenuName = nullptr; //Указатель на меню
	wc.style = CS_VREDRAW | CS_HREDRAW;	// Стиль окна

	if (!RegisterClassEx(&wc))				//Регистрируем окно
	{
		return EXIT_FAILURE;
	}

	if (hwnd = CreateWindow (wc.lpszClassName, L"Заголовок окна", WS_OVERLAPPEDWINDOW, 0, 0, 600, 600, nullptr/*Т.к нет родительского окна, ставим nullptr*/, nullptr, wc.hInstance, nullptr); 
		hwnd == INVALID_HANDLE_VALUE)	//Создаем окно, иначе кидаем ошоибку
	{
		return EXIT_FAILURE;
	}

	ShowWindow(hwnd, nCmdShow);	//Показать окно
	UpdateWindow(hwnd);		//Перерисовать окно


	while (GetMessage(&msg, nullptr, 0, 0))			//Запуск цикла обработки сообщений
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						//Передает сообщение в оконную обработку
	}
	


	return static_cast<int>(msg.wParam);
	}