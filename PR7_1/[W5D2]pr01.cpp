
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

HWND ChildHwnd[2];
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	
	static HWND hwndClient;
	CLIENTCREATESTRUCT clientcreate;
	MDICREATESTRUCT mdicreate;
	HWND hwndChild;

	static BOOL split;
	static HCURSOR hCursor;
	static int boundary = -1;
	RECT rectView;
	static int num = 0;

	TCHAR str[30];

	OPENFILENAME OFN;

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

		case ID_OPEN:
			mdicreate.szClass =
				_T("Child Window Class Name");

			_stprintf_s(str, 30, _T("%d¹ø Child Window"), ++num);
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
			return 0;
		case ID_FILENEW:
			mdicreate.szClass =
				_T("Child Window Class Name");

			_stprintf_s(str, 30, _T("%d¹ø Child Window"), ++num);
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
			return 0;

		case ID_WINDOW_CLOSE:
			hwndChild = (HWND)SendMessage(hwndClient, WM_MDIGETACTIVE, 0, 0);
			if (SendMessage(hwndChild, WM_QUERYENDSESSION, 0, 0))
				SendMessage(hwndClient, WM_MDIDESTROY, (WPARAM)hwndChild, 0);
			return 0;
		case ID_TILE:
			SendMessage(hwndClient, WM_MDITILE, 0, 0);
			return 0;
		case ID_CASCADE:
			SendMessage(hwndClient, WM_MDICASCADE, 0, 0);
			return 0;
		case ID_ARRANGE:
			SendMessage(hwndClient, WM_MDIICONARRANGE, 0, 0);
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
	

	//switch (iMsg) {
	//case WM_CREATE:
	//	split = FALSE;
	//	return 0;
	//case WM_COMMAND:
	//	switch (LOWORD(wParam)) {
	//	case ID_SPLIT_2X1:
	//		if (split == TRUE) break;
	//		split = TRUE;
	//		GetClientRect(hwnd, &rectView);
	//		ChildHwnd[0] = CreateWindowEx(
	//			WS_EX_CLIENTEDGE,
	//			_T("Child Window Class Name"),
	//			NULL,
	//			WS_CHILD | WS_VISIBLE,
	//			0,
	//			0,
	//			rectView.right,
	//			rectView.bottom / 2 - 1,
	//			hwnd,
	//			NULL,
	//			hInst,
	//			NULL
	//		);

	//		ChildHwnd[1] = CreateWindowEx(
	//			WS_EX_CLIENTEDGE,
	//			_T("Child Window Class Name"),
	//			NULL,
	//			WS_CHILD | WS_VISIBLE,
	//			0,
	//			rectView.bottom / 2 + 1,
	//			rectView.right,
	//			rectView.bottom / 2 - 1,
	//			hwnd,
	//			NULL,
	//			hInst,
	//			NULL
	//		);
	//		boundary = rectView.bottom / 2;
	//		hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS));
	//		return 0;
	//	case ID_EXIT:
	//		PostQuitMessage(0);
	//		return 0;
	//	}
	//	case WM_MOUSEMOVE:
	//		if (HIWORD(lParam) >= boundary - 2 && 
	//				HIWORD(lParam) <= boundary + 2) SetCursor(hCursor);
	//		if (wParam == MK_LBUTTON && split == TRUE) {
	//			GetClientRect(hwnd, &rectView);
	//			if (rectView.top + 5 < HIWORD(lParam) &&
	//				HIWORD(lParam) <
	//				rectView.bottom - 5)boundary = HIWORD(lParam);
	//			MoveWindow(ChildHwnd[0], 0, 0, rectView.right, boundary - 1, TRUE);
	//			MoveWindow(ChildHwnd[1], 0, boundary + 1, rectView.right, rectView.bottom - boundary + 1, TRUE);
	//		}
	//		return 0;
	//	case WM_LBUTTONDOWN:
	//		if (split == TRUE) {
	//			SetCursor(hCursor);
	//			SetCapture(hwnd);
	//		}return 0;
	//	case WM_LBUTTONUP:
	//		if (split == TRUE) ReleaseCapture();
	//		return 0;
	//	
	//	return 0;
	//case WM_SIZE:
	//	if (split == TRUE) {
	//		GetClientRect(hwnd, &rectView);
	//		MoveWindow(ChildHwnd[0], 0, 0, rectView.right,
	//			rectView.bottom / 2 - 1, TRUE);
	//		MoveWindow(ChildHwnd[1], 0, rectView.bottom / 2 + 1,
	//			rectView.right, rectView.bottom / 2 - 1, TRUE);
	//	}
	//	return 0;

	//case WM_DESTROY:
	//	PostQuitMessage(0);
	//	return 0;
	//}
	//return DefWindowProc(hwnd, iMsg, wParam, lParam);

}


LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{

	HDC hdc;
	static int x[2] = { 20, 20 }, y[2] = { 20, 20 }, flag[2];
	int select;
	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_TIMER:
		x[wParam] = x[wParam] + 20;
		hdc = GetDC(hwnd);
		Ellipse(hdc, x[wParam] - 20, y[wParam] - 20, x[wParam] + 20, y[wParam] + 20);
		ReleaseDC(hwnd, hdc);
		break;

	case WM_LBUTTONDOWN:
		if (hwnd == ChildHwnd[0])
			select = 0;
		else
			select = 1;
		flag[select] = 1 - flag[select];
		if (flag[select])
			SetTimer(hwnd, select, 100, NULL);
		else
			KillTimer(hwnd, select);
		break;
	//case WM_PAINT:

	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}