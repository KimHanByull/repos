#include <windows.h>
#include <TCHAR.H>
#include "resource.h"
#include <time.h>
#include <vector>
#include <algorithm>
#include <random>

#define WIDTH 819
#define HEIGHT 614


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInst;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

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
	if (!RegisterClass(&wndclass)) return 1;


	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("김한별의 다섯 번째 윈도우"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);

	srand(static_cast<unsigned int>(time(nullptr)));

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	HDC hdc, memdc;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap;
	static char dividenum;
	static int width, height;


	switch (message) {
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		dividenum = 'A';
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBitmap);
		BitBlt(hdc, 0, 0, WIDTH, HEIGHT, memdc, 0, 0, SRCCOPY);



		switch (dividenum) {
		case 'T':
			for (int j = 0; j < 3; j++) {
				for (int i = 0; i < 3; i++) {
				

				}
			}
			break;
		case 'F':
			for (int j = 0; j < 4; j++) {
				for (int i = 0; i < 4; i++) {

				}
			}
			break;
		case 'D':
		{
			int divisionWidth = WIDTH / 5;
			int divisionHeight = HEIGHT / 5;
			const int numDivisions = 5;

			std::vector<RECT> divisions;

			for (int j = 0; j < numDivisions; j++) {
				for (int i = 0; i < numDivisions; i++) {
					RECT divisionRect;
					divisionRect.left = i * divisionWidth;
					divisionRect.top = j * divisionHeight;
					divisionRect.right = divisionRect.left + divisionWidth;
					divisionRect.bottom = divisionRect.top + divisionHeight;

					divisions.push_back(divisionRect);
				}
			}

			// Shuffle the divisions vector to randomize positions
			std::shuffle(divisions.begin(), divisions.end(), std::default_random_engine(time(0)));

			// Draw each division at its randomized position
			for (const RECT& divisionRect : divisions) {
				BitBlt(hdc, divisionRect.left, divisionRect.top,
					divisionWidth, divisionHeight, memdc,
					divisionRect.left, divisionRect.top, SRCCOPY);
			}
		}
			break;
		}



		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;


	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_3BY3:
			width = WIDTH / 3;
			height = HEIGHT / 3;
			dividenum = 'T';
			break;


		case ID_4BY4:
			dividenum = 'F';
			break;

		case ID_5BY5:
			width = WIDTH / 5;  // 추가된 부분
			height = HEIGHT / 5;  // 추가된 부분
			dividenum = 'D';
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);

}