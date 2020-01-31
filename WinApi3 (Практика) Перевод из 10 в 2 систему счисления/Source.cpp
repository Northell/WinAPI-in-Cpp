#include "Source.h"

CApp::CApp() {
	try {
		this->init_native_window_obj();
		this->create_native_controls();

	}
	catch(const exception& e)
	{
		string exp_data = e.what();
		MessageBox(nullptr, wstring(begin(exp_data), end(exp_data)).c_str(), L"Ошибка", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
}

CApp::~CApp()
{

}

void CApp::init_native_window_obj()
{
	//===========
	/*Из WinApi1*/
	WNDCLASSEX _wc{ sizeof(WNDCLASSEX) }; //Отвечает за характеристики окна
	_wc.cbClsExtra = 0;
	_wc.cbWndExtra = 0;			//Отвечают за выделение памяти в классе нашего окна

	_wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));			//цвет окраски окна
	_wc.hCursor = LoadCursor(nullptr, IDC_ARROW);			//Курсор
	_wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);			//Иконка
	_wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);		//маленькая иконка для отображения в заголовке окна
	_wc.hInstance = GetModuleHandle(nullptr);								//
	_wc.lpfnWndProc/*Это процедура(Указатель) отвечает за обработку сообщений*/ = CApp::application_proc;
	_wc.lpszClassName = this->m_szClassName.c_str();
	_wc.lpszMenuName = nullptr; //Указатель на меню
	_wc.style = CS_VREDRAW | CS_HREDRAW;	// Стиль окна

	if (!RegisterClassEx(&_wc))				//Регистрируем окно
	{
		throw runtime_error("Error, can't register mainWindow class"s);
	}
	//=======
	/*Отвечает за выравнивание клиентской части окна*/
	RECT _windowRC{ 0, 0, this->m_nAppWidth, this->m_nAppHeight };
	AdjustWindowRect(&_windowRC, WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE, false);
	//=========

	if (this->m_hWnd = CreateWindowEx(
		0,
		this->m_szClassName.c_str(),
		this->m_szAppName.c_str(),
		WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE,
		(GetSystemMetrics(SM_CXSCREEN) - _windowRC.right) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - _windowRC.bottom) / 2,
		_windowRC.right, _windowRC.bottom, nullptr, nullptr, nullptr, this);
		!this->m_hWnd)
		throw runtime_error("Error, can't create main window"s);

}



void CApp::create_native_controls()
{
	if (this->m_hWndButton = CreateWindowEx(
		0,
		L"BUTTON",
		L"Перевести",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		56, 108, 238, 37, this->m_hWnd, reinterpret_cast<HMENU>(CApp::CTL_ID::CALCBUTTON_ID), nullptr, nullptr);
		!this->m_hWndButton)
		throw runtime_error("Error, can't create button!"s);

	if (this->m_hWndEdit = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		L"256",
		WS_CHILD  | WS_VISIBLE| ES_AUTOHSCROLL,
		56,66,238,24, this->m_hWnd, reinterpret_cast<HMENU>(CApp::CTL_ID::RESULTEDIT_ID), nullptr, nullptr);
		!this->m_hWndEdit)
		throw runtime_error("Error, can't create edit!"s);
	HFONT hFont = CreateFont(18, 0, 0, 0, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Roboto"); //Изменение шрифта элементов управления
	SendMessage(this->m_hWndEdit, WM_SETFONT/*Устанавливает шрифт*/, reinterpret_cast<WPARAM>(hFont), TRUE);
	SendMessage(this->m_hWndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
}

int CApp::Run()
{
	MSG _msg;
	 //===============
	ShowWindow(this->m_hWnd, SW_SHOWDEFAULT);		//Вызываем их здесь, чтобы сначала приложение рендерилось, затем его видел польлзователь, а не наоброт
	UpdateWindow(this->m_hWnd);
	//================
	while (GetMessage(&_msg, nullptr, 0, 0))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}
	return static_cast<int>(_msg.wParam);
}

LRESULT CApp::application_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CApp* pApp;
	if (uMsg == WM_NCCREATE)
	{
		pApp = static_cast<CApp*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp)))
		{
			if (GetLastError()!=0)
				return false;
		}
	}
	else
	{
		pApp = reinterpret_cast<CApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pApp)
	{
		pApp->m_hWnd = hWnd;
		return pApp->window_proc(hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

LRESULT CApp::window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_COMMAND:
	{
		switch (static_cast<CApp::CTL_ID>(LOWORD(wParam))) 
		{
		case CApp::CTL_ID::CALCBUTTON_ID:
		{
			try
			{
				wstring text;
				text.resize(MAX_PATH);
				GetWindowText(this->m_hWndEdit, &text[0], MAX_PATH);
				text.erase(remove(begin(text), end(text), 0), end(text));//Убирает все незначительные нули(байты)
				if (text.empty())
				{
					MessageBox(this->m_hWnd, L"Введите число", L"Информация", MB_ICONINFORMATION | MB_OK);
					break;
				}
				string binary = bitset<16>(stoi(text)).to_string();
				text = wstring(begin(binary), end(binary));
				SetWindowText(this->m_hWndEdit, text.c_str());

			}
			catch (...)
			{
				MessageBox(this->m_hWnd, L"Число превышает 2 байта!", L"Предупреждение", MB_ICONWARNING | MB_OK);
			}
			break;
		}
		}
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(EXIT_SUCCESS);
	}
	return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
