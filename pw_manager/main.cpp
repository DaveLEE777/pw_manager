#include<windows.h>

#include "Pw_Gen.h"
#include "ByteToChar.h"

#define Pw_Gen_button 1001

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 진입점
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
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
		20, 20, 120, 30,
		hWnd,
		(HMENU)Pw_Gen_button,
		hInstance,
		NULL
	);
	
	// 생성된 패스워드 출력
	HWND hEditOutput = CreateWindow(
		L"EDIT",
		L"testtesttest",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
		160, 20, 400, 30,
		hWnd,
		(HMENU)2,
		hInstance,
		NULL
	);
	
	// 복사 버튼

	HWND hButton_Copy = CreateWindow(
		L"BUTTON",
		L"COPY",
		WS_CHILD | WS_VISIBLE ,
		800, 20, 120, 30,
		hWnd,
		(HMENU)3,
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
			switch (LOWORD(wParam)) {
				case Pw_Gen_button:
					byte space[32] = { 0, };
					wchar_t ppw[33] = {0,};
					Pw_Gen(space, 32);
					ByteToChar(space,ppw,32);
					MessageBox(hWnd, ppw, L"Result", MB_OK);
					
					break;
			}

		
		
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
} 

