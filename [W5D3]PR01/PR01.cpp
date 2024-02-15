#include <windows.h>
#include <tchar.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam);
void ReadFromFile(int WndIndex, TCHAR filename[]);
HWND hwndChild[100];
TCHAR WinBuff[100][1000];
int WndCount;
HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wndclass;
	HWND hwnd;
	MSG msg;
	hInst = hInstance;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wndclass.hCursor = LoadCursor(NULL, IDC_IBEAM);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wndclass.lpszClassName = TEXT("HelloClass");
	RegisterClass(&wndclass);
	wndclass.lpfnWndProc = ChildWndProc;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = _T("Child Window Class Name");
	RegisterClass(&wndclass);
	hwnd = CreateWindow(
		TEXT("HelloClass"),
		TEXT("김한별의 여덟 번째 윈도우"),
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
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam) {
	//----------메모장 읽어오기-----------------------
	HDC hdc;
	HANDLE hFile;
	TCHAR InBuff[1000];
	TCHAR OutBuff[100] = _T("API 프로그래밍을 못해도 사랑합니다.");
	DWORD size;
	RECT rt;
	//----------메모장 읽어오기-----------------------
	static HWND hwndClient;
	CLIENTCREATESTRUCT clientcreate;
	MDICREATESTRUCT mdicreate;
	static TCHAR filepath[1000], folder[100], filename[100];
	TCHAR filter[] = _T("텍스트 파일 (*.txt)\0*.txt\0모든 파일 (*.*)\0*.*\0");
	OPENFILENAME OFN;

	switch (iMsg) {
	case WM_CREATE:
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hwnd), 0);
		clientcreate.idFirstChild = 100;
		hwndClient = CreateWindow(
			_T("MDICLIENT"),
			NULL,
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
		case ID_FILEOPEN:
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFile = filepath;
			OFN.nMaxFileTitle = 100;
			OFN.lpstrFileTitle = filename;
			OFN.nMaxFile = 1000;
			OFN.lpstrFilter = filter;
			OFN.Flags = OFN_EXPLORER;
			GetOpenFileName(&OFN);
			WndCount++;
			ReadFromFile(WndCount, filepath);
			mdicreate.szClass = _T("Child Window Class Name");
			mdicreate.szTitle = filepath;
			mdicreate.hOwner = hInst;
			mdicreate.x = CW_USEDEFAULT;
			mdicreate.y = CW_USEDEFAULT;
			mdicreate.cx = CW_USEDEFAULT;
			mdicreate.cy = CW_USEDEFAULT;
			mdicreate.style = 0;
			mdicreate.lParam = 0;
			hwndChild[WndCount] = (HWND)SendMessage(hwndClient,
				WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);

			ShowWindow(hwndChild[WndCount], SW_SHOWNORMAL);
			return 0;
		}
		break;
		/*case WM_LBUTTONDOWN:

		//----------메모장 읽어오기-----------------------
		hFile = CreateFile(
			_T("test1.txt"),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, 0, 0
		);
		memset(InBuff, 0, sizeof(InBuff));
		ReadFile(hFile, InBuff, 999 * sizeof(TCHAR), &size, NULL);
		hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rt);
		DrawText(hdc, InBuff, (int)_tcslen(InBuff), &rt, DT_TOP | DT_LEFT);
		ReleaseDC(hwnd, hdc);
		WriteFile(hFile, OutBuff, (DWORD)_tcslen(OutBuff) * sizeof(TCHAR), &size, NULL);
		CloseHandle(hFile);
		//----------메모장 읽어오기-----------------------

		break;
	case WM_PAINT:
		break;*/
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void ReadFromFile(int WndIndex, TCHAR filename[]) {
	HANDLE hFile;
	DWORD size = 1000;
	hFile = CreateFile(
		filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL, OPEN_EXISTING, 0, 0
	);
	ReadFile(hFile, WinBuff[WndIndex], size * sizeof(TCHAR), &size, NULL);
	WinBuff[WndIndex][size] = NULL;
	CloseHandle(hFile);
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	int i, SelectWnd;
	HDC hdc;
	RECT rt;
	PAINTSTRUCT ps;

	for (i = 1; i <= WndCount; i++)
		if (hwnd == hwndChild[i]) {
			SelectWnd = i;
			break;
		}
	switch (iMsg) 
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rt);
		DrawText(hdc, WinBuff[SelectWnd], (int)_tcslen(WinBuff[SelectWnd]),
			&rt, DT_TOP | DT_LEFT);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
