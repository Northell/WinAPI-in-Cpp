
//=====================
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//=====================

#include <windows.h>;

int width;
int height;
int CALLBACK wWinMain(HINSTANCE hinstance,// ��������� �� ������ ������������� ������
	HINSTANCE,//�������� ������ ��� 16 ������ ����������
	PWSTR szCadLine,// Pointer to white string, ������ ���������, ������� ���������� ���������
	int nCmdShow)	// �������� �� ��� ����, ���������, ����������� ���
{
	MSG msg{}; // ���������� ��� ��������� ��������� ����� ��������� WinApi
	HWND hwnd{}; //��� �������� ���� � ������ ����
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) }; //�������� �� �������������� ����
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;			//�������� �� ��������� ������ � ������ ������ ����

	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));			//���� ������� ����
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);			//������
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);			//������
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);		//��������� ������ ��� ����������� � ��������� ����
	wc.hInstance = hinstance;								//
	wc.lpfnWndProc/*��� ���������(���������) �������� �� ��������� ���������*/ = [](HWND hWND,/*���������� ����, �������� �������� ���������*/
		UINT uMsg,/*��� ���������*/
		WPARAM wParam, LPARAM lParam/*� wParam � lParam ����������� ���� ��� ���������*/)->LRESULT
	{
		switch (uMsg)
		{
			//=======================
			/* ��� ��������� ���������� �����, ���� � ��� ��������� ����*/
		case WM_CREATE:
		{
			HWND hButton = CreateWindow(
				L"Button",
				L"Ok",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				0, 0, 100,30, hWND, reinterpret_cast<HMENU>(1337)/*���������� ������������� ��� ��������� � ������*/, nullptr, nullptr);
		}

		case WM_SIZE:
		{
			width = LOWORD(lParam);
			height = HIWORD(lParam);
			//MessageBeep(0);
		}
		return 0;

		case WM_COMMAND:		/*������������, ����� ���������� ����� �� ������� � ����*/
		{
			switch (LOWORD(wParam))
			{
			case 1337:
			{
				MessageBox(hWND, L"Hi", L"Ok", MB_ICONINFORMATION);
			}
			break;

			}
		}
		return 0;
			//=========================
		case WM_DESTROY:
		{
			PostQuitMessage(EXIT_SUCCESS);
		}

		return 0;

		}
		return DefWindowProc(hWND, uMsg, wParam, lParam); //�����, ���� ��������� ����� �� ��������������
	};
	wc.lpszClassName = L"MyAppClass";
	wc.lpszMenuName = nullptr; //��������� �� ����
	wc.style = CS_VREDRAW | CS_HREDRAW;	// ����� ����

	if (!RegisterClassEx(&wc))				//������������ ����
	{
		return EXIT_FAILURE;
	}

	if (hwnd = CreateWindow(wc.lpszClassName, L"��������� ����", WS_OVERLAPPEDWINDOW, 0, 0, 600, 600, nullptr/*�.� ��� ������������� ����, ������ nullptr*/, nullptr, wc.hInstance, nullptr);
		hwnd == INVALID_HANDLE_VALUE)	//������� ����, ����� ������ �������
	{
		return EXIT_FAILURE;
	}

	ShowWindow(hwnd, nCmdShow);	//�������� ����
	UpdateWindow(hwnd);		//������������ ����


	while (GetMessage(&msg, nullptr, 0, 0))			//������ ����� ��������� ���������
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						//�������� ��������� � ������� ���������
	}



	return static_cast<int>(msg.wParam);
}