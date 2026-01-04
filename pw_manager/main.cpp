#include<windows.h>
#include<CommCtrl.h>

#pragma comment(lib, "comctl32.lib")


#include "Pw_Gen.h"
#include "ByteToChar.h"
#include "WcharToInt.h"

#define Pw_Gen_button 1001
#define Show_Pw 1002
#define Len_Combo 1003
#define COMBO_INIT (WM_APP + 1)
#define Max_Len 64 // 난수 최대 길이
#define Copy_Button 1004
#define Show_Id 1005


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND g_hEditPw = NULL;
HWND g_hLenCombo = NULL;
HWND g_hEditId = NULL;

int LenSetting = 32; // 난수 길이 설정


// 진입점
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	INITCOMMONCONTROLSEX icc = {};
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_STANDARD_CLASSES;
	InitCommonControlsEx(&icc);
	
	
	
	
	//윈도우 클래스 등록
	//윈도우의 설계도
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"MyWindowClass";

	RegisterClass(&wc);

	//최상위 윈도우 생성
	HWND hWnd = CreateWindow(
		L"MyWindowClass",
		L"Password Manager",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1000, 500,
		NULL, NULL, hInstance, NULL
	);

	// 생성 버튼 
	HWND hButton = CreateWindow(
		L"BUTTON",
		L"Generate",
		WS_CHILD | WS_VISIBLE,
		60, 20, 120, 30,
		hWnd,
		(HMENU)Pw_Gen_button,
		hInstance,
		NULL
	);

	// 길이 선택 옵션
	g_hLenCombo = CreateWindow(
		L"COMBOBOX",
		NULL,
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		10, 20, 50, 100,
		hWnd,
		(HMENU)Len_Combo,
		hInstance,
		NULL
	);

	PostMessage(hWnd, COMBO_INIT, 0, 0);
	


	// 생성된 패스워드 출력 (핸들 전역임)
	g_hEditPw = CreateWindow(
		L"EDIT",
		L"Please push the 'Generate' button to make the new PW",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
		200, 20, 600, 30,
		hWnd,
		(HMENU)Show_Pw,
		hInstance,
		NULL
	);

	// ID 입력란
	g_hEditId = CreateWindow(
		L"EDIT",
		L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		200, 60, 600, 30,
		hWnd,
		(HMENU)Show_Id,
		hInstance,
		NULL
	);
	
	// ID 입력란용 플레이스 홀더
	SendMessage(
		g_hEditId,
		EM_SETCUEBANNER,
		TRUE,   // TRUE면 포커스 받아도 유지
		(LPARAM)L"Please Enter the ID"
	);


	// 복사 버튼

	HWND hButton_Copy = CreateWindow(
		L"BUTTON",
		L"COPY",
		WS_CHILD | WS_VISIBLE ,
		820, 20, 120, 30,
		hWnd,
		(HMENU)Copy_Button,
		hInstance,
		NULL
	);
	
	
	// 메세지 루프
	ShowWindow(hWnd, nCmdShow);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case COMBO_INIT:
	{
		SendMessage(g_hLenCombo, CB_ADDSTRING, 0, (LPARAM)L"16");
		SendMessage(g_hLenCombo, CB_ADDSTRING, 0, (LPARAM)L"24");
		SendMessage(g_hLenCombo, CB_ADDSTRING, 0, (LPARAM)L"32");
		SendMessage(g_hLenCombo, CB_ADDSTRING, 0, (LPARAM)L"64");

		SendMessage(g_hLenCombo, CB_SETCURSEL, 2, 0);
		return 0;
	}

	case WM_COMMAND:
	{
		WORD id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if (id == Pw_Gen_button && code == BN_CLICKED)
		{
			BYTE space[Max_Len] = {};
			wchar_t ppw[Max_Len+1] = {};
			Pw_Gen(space, LenSetting);
			ByteToChar(space, ppw, LenSetting);
			SetWindowTextW(g_hEditPw, ppw);
			return 0;
		}
		
		// 복사 버튼
		if (id == Copy_Button && code == BN_CLICKED) {
			SendMessage(g_hEditPw, EM_SETSEL, 0, -1);
			SendMessage(g_hEditPw, WM_COPY, 0, 0);

		}




		// 길이 버튼
		if (id == Len_Combo && code == CBN_SELCHANGE)
		{
			int sel = (int)SendMessage(g_hLenCombo, CB_GETCURSEL, 0, 0);
			if (sel != CB_ERR)
			{
				wchar_t buf[8] = {};
				SendMessage(g_hLenCombo, CB_GETLBTEXT, sel, (LPARAM)buf); // 반환값 buf 에 저장
				LenSetting = WcharToInt(buf);
				
			}
			return 0;
		}

		
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
