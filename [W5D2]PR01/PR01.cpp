
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
		TEXT("���Ѻ��� 7��° ������"),
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

HWND ActivatedHwndChild;

int selection;
int childCount;

static struct Child {
	HWND hwndChild;
	TCHAR str[10][100] = { '\0', };
	int count = 0, line = 0;
};

Child child[10];


LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{

	static HWND hwndClient;
	CLIENTCREATESTRUCT clientcreate;
	MDICREATESTRUCT mdicreate;

	TCHAR str[30];		//�ڽ� �����찡 �����Կ� ���� ��ȣ ���̱� ����


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
			_stprintf_s(str, 30, _T("%d�� Child Window"), childCount);
			mdicreate.szTitle = str;
			mdicreate.hOwner = hInst;
			mdicreate.x = CW_USEDEFAULT;
			mdicreate.y = CW_USEDEFAULT;
			mdicreate.cx = CW_USEDEFAULT;
			mdicreate.cy = CW_USEDEFAULT;
			mdicreate.style = 0;
			mdicreate.lParam = 0;
			child[childCount++].hwndChild = (HWND)SendMessage(hwndClient,
				WM_MDICREATE, 0,
				(LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			return 0;
		case ID_EXIT:
			PostQuitMessage(0);
			return 0;
		}
		break;
	case WM_MOUSEACTIVATE:
		ActivatedHwndChild = (HWND)SendMessage(hwndClient, WM_MDIGETACTIVE, 0, 0);
		selection = -1; // �ʱ�ȭ
		for (int i = 0; i < childCount; i++) {
			if (ActivatedHwndChild == child[i].hwndChild) {
				selection = i;
				break;
			}
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
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR tmp[20];
	switch (iMsg)
	{
	case WM_CREATE:
		_stprintf_s(tmp, 20, _T("%d"), wParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(ActivatedHwndChild, &ps);
		if (hdc != NULL) {
			for (int i = 0; i <= child[selection].line; i++) {
				TextOut(hdc, 0, i * 20, child[selection].str[i], _tcslen(child[selection].str[i]));
			}
			EndPaint(ActivatedHwndChild, &ps);
		}
		break;

	case WM_CHAR:
		hdc = GetDC(hwnd);
		if (child[selection].line < 10) {
			if (wParam == VK_BACK && child[selection].count > 0) {
				child[selection].count--;
				child[selection].str[child[selection].line][child[selection].count] = NULL; // �齺���̽� �� �ش� ��ġ�� ���ڸ� '\0'���� ����
			}
			else if (wParam == VK_BACK && child[selection].count == 0 && child[selection].line > 0) {
				child[selection].line--;
				child[selection].count = _tcslen(child[selection].str[child[selection].line]); // ���� ���� ������ ���ں��� ����
				if (child[selection].count > 0) {
					child[selection].count--; // ���� ���� ������ ���ڿ� ���� �ε����� �̵�
					child[selection].str[child[selection].line][child[selection].count] = NULL; // ������ ���ڸ� '\0'���� ����
				}
			}
			else {
				child[selection].str[child[selection].line][child[selection].count++] = wParam;
			}
			ReleaseDC(hwnd, hdc);
			InvalidateRect(ActivatedHwndChild, NULL, TRUE);
		}
		break;



	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {
			// Enter Ű�� ������ ���� ������ �̵�
			child[selection].line++;
			child[selection].count = 0;
			if (child[selection].line < 10) {
				child[selection].str[child[selection].line][0] = _T('\0'); // ���� ���� ù ���ڸ� '\0'���� ���� <- ����Ű �Է� �� l������ ���� ���� ����
				InvalidateRect(ActivatedHwndChild, NULL, TRUE);
			}
		}

		break;
	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}
