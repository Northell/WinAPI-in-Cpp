#pragma once
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <string>
#include <algorithm>
#include <bitset>
#include <exception>

using namespace std;
class CApp
{
	enum class CTL_ID {
		CALCBUTTON_ID,
		RESULTEDIT_ID
	};		//�������� �������������� ����� ���������
public:
	explicit CApp();			// ����������� ����
	~CApp();					//����������

	int Run();					// �������� �� ������?
private:
	void init_native_window_obj();																	//������������� ����
	
	static LRESULT CALLBACK application_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //����������� ������� ���������, ������� ����� ��������� �������� ������ ������������ ������� ���������, ������� ����� ������ � ����� ������.
	LRESULT CALLBACK window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);			  // ������������ ������� ���������

	void create_native_controls();																//�������� �� �������� ��������� ����������

	//======================
	/*��� ��������� ���������, ����������� ����*/
private:
	const wstring m_szAppName{ L"Bin-VV" }, m_szClassName{ L"Bin-VV_Window_class_nostyles" };
	HWND m_hWnd{}, m_hWndButton{}, m_hWndEdit{};
	const int m_nAppWidth{ 350 }, m_nAppHeight{ 250 };
	//===================

};



