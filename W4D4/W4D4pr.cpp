#include <windows.h>
#include "resource.h";


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASS wndclass;
	HWND hwnd;
	MSG msg;


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
	if (!RegisterClass(&wndclass)) return 1;
	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("김한별의 네 번째 윈도우"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, SW_MAXIMIZE);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

#include <math.h>
#define BSIZE 40


double LengthPts(int x1, int y1, int x2, int y2)
{
	return(sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

BOOL InCircle(int x, int y, int mx, int my)
{
	if (LengthPts(x, y, mx, my) < BSIZE) return TRUE;
	else return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HMENU hMenu, hSubMenu;
	int mx, my;
	static BOOL Select;
	static BOOL Copy;
	static int x, y;
	static int startX, startY, oldX, oldY;
	static BOOL Drag;
	int endX, endY;
	static COLORREF tmpFace[16], colorFace, tmpPen[16], colorPen;
	CHOOSECOLOR COLORFACE, COLORPEN;

	switch (iMsg)
	{
	case WM_CREATE:
		hMenu = GetMenu(hwnd);
		hSubMenu = GetSubMenu(hMenu, 1);
		//Select = FALSE;
		//Copy = FALSE;
		x = 50;	y = 50;
		Drag = FALSE;
		return 0;
	case WM_PAINT:
		/*	EnableMenuItem(hSubMenu, ID_EDITCOPY,
				Select ? MF_ENABLED : MF_GRAYED);
			EnableMenuItem(hSubMenu, ID_EDITPASTE,
				Copy ? MF_ENABLED : MF_GRAYED);*/
		hdc = BeginPaint(hwnd, &ps);
		/*	if (Select)
				Rectangle(hdc, x - BSIZE, y - BSIZE, x + BSIZE, y + BSIZE);
			Ellipse(hdc, x - BSIZE, y - BSIZE, x + BSIZE, y + BSIZE);
			*/
		MoveToEx(hdc, startX, startY, NULL);
		LineTo(hdc, oldX, oldY);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_COMMAND:

		switch (LOWORD(wParam)) {

		case ID_LINE:

			break;

		case ID_RECTANGLE:


		case ID_ELLIPSE:



		case ID_EDITCOPY:
			Copy = TRUE;
			InvalidateRgn(hwnd, NULL, TRUE);
			break;

		case ID_PENCOLOR:
			for (int i = 0; i < 16; i++)
				tmpPen[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
			memset(&COLORPEN, 0, sizeof(CHOOSECOLOR));
			COLORPEN.lStructSize = sizeof(CHOOSECOLOR);
			COLORPEN.hwndOwner = hwnd;
			COLORPEN.lpCustColors = tmpPen;
			COLORPEN.Flags = CC_FULLOPEN;
			if (ChooseColor(&COLORPEN) != 0)
			{
				colorPen = COLORPEN.rgbResult;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			break;

		case ID_FACECOLOR:
			for (int i = 0; i < 16; i++)
				tmpFace[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
			memset(&COLORFACE, 0, sizeof(CHOOSECOLOR));
			COLORFACE.lStructSize = sizeof(CHOOSECOLOR);
			COLORFACE.hwndOwner = hwnd;
			COLORFACE.lpCustColors = tmpFace;
			COLORFACE.Flags = CC_FULLOPEN;
			if (ChooseColor(&COLORFACE) != 0)
			{
				colorFace = COLORFACE.rgbResult;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			break;
		}


	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InCircle(x, y, mx, my)) Select = TRUE;
		startX = mx;
		startY = my;
		Drag = TRUE;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONUP:

		startX = oldX;
		startY = oldY;
		Drag = FALSE;
		break;

	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		if (Drag)
		{
			SetROP2(hdc, R2_XORPEN);
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY);	// 지우기
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);	// 그리기
			oldX = endX; oldY = endY;
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
