
#include <windows.h>
#include "resource.h"
#include <stdio.h>
#include <tchar.h>

LRESULT CALLBACK FrameWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {


	WNDCLASS wndclass;
	HWND hwnd;
	MSG msg;
	hInst = hInstance;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = FrameWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wndclass.hCursor = LoadCursor(NULL, IDC_IBEAM);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wndclass.lpszClassName = TEXT("Window Class Name");
	RegisterClass(&wndclass);
	wndclass.lpfnWndProc = ChildWndProc;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = _T("Child Window Class Name");
	RegisterClass(&wndclass);
	hwnd = CreateWindow(
		TEXT("Window Class Name"),
		TEXT("Main Window Title"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

HWND ChildHwnd[100];
static int num = 0;
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{

	static HWND hwndClient;
	CLIENTCREATESTRUCT clientcreate;
	MDICREATESTRUCT mdicreate;
	HWND hwndChild;


	TCHAR str[30];		//자식 윈도우가 증가함에 따라 번호 붙이기 위함


	switch (iMsg) {
	case WM_CREATE:
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hwnd), 0);
		clientcreate.idFirstChild = 100;
		hwndClient = CreateWindow(
			_T("MDICLIENT"), NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
			0, 0, 0, 0,
			hwnd,
			NULL,
			hInst,
			(LPSTR)&clientcreate
		);
		ShowWindow(hwndClient, SW_SHOW);
		return 0;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_NEWFILE:
			mdicreate.szClass =
				_T("Child Window Class Name");
			_stprintf_s(str, 30, _T("%d번 Child Window"), ++num);
			mdicreate.szTitle = str;
			mdicreate.hOwner = hInst;
			mdicreate.x = CW_USEDEFAULT;
			mdicreate.y = CW_USEDEFAULT;
			mdicreate.cx = CW_USEDEFAULT;
			mdicreate.cy = CW_USEDEFAULT;
			mdicreate.style = 0;
			mdicreate.lParam = 0;
			hwndChild = (HWND)SendMessage(hwndClient,
				WM_MDICREATE, 0,
				(LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			ChildHwnd[num] = hwndChild;
			//num 값 비교하여 같을 경우에만 출력
			return 0;
		case ID_EXIT:
			PostQuitMessage(0);
			return 0;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefFrameProc(hwnd, hwndClient, iMsg, wParam, lParam);


}
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static TCHAR content[100][10][100];    // 입력된 문장을 저장하기 위함
	static int count;
	static int enterCount = 0, yPos = 0;
	RECT rt = { 0, 0, 1000, 1000 };

	HDC hdc;
	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i <= enterCount; i++) {
			DrawText(hdc, content[num][i], _tcslen(content[num][i]), &rt, DT_TOP | DT_LEFT);
			rt.top += 20;  // 행 간격 조절
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		if (hwnd == ChildHwnd[num]) {
			if (wParam == VK_BACK && count > 0) {
				count--;
			}
			else {
				if (enterCount < 9 && count < 99) {
					content[num][enterCount][count++] = wParam;
					if (count >= 99) {
						// count가 99에 도달하면 다음 행으로 이동
						enterCount++;
						count = 0;
						if (enterCount >= 10) {
							// 행이 10을 넘어가면 마지막 행을 초기화하고 이전 행들을 한 칸씩 위로 이동
							_tcscpy_s(content[num][9], _T(""));
							for (int i = 0; i < 9; i++) {
								_tcscpy_s(content[num][i], content[num][i + 1]);
							}
							enterCount = 9;
						}
					}
				}
			}
		}
		content[num][enterCount][count] = NULL;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {
			// Enter 키를 누르면 다음 행으로 이동
			enterCount++;
			count = 0;
			if (enterCount >= 10) {
				// 행이 10을 넘어가면 마지막 행을 초기화하고 이전 행들을 한 칸씩 위로 이동
				_tcscpy_s(content[num][9], _T(""));
				for (int i = 0; i < 9; i++) {
					_tcscpy_s(content[num][i], content[num][i + 1]);
				}
				enterCount = 9;
			}
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}
