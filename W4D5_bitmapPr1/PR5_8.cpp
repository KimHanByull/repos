#include <windows.h>
#include <TCHAR.H>
#include "resource1.h"
#include <time.h>
#include <stdlib.h>
#include <algorithm>


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


	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("���Ѻ��� �ټ� ��° ������"),
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


int GetRandomIndex(int maxIndex) {
	return rand() % maxIndex;
}


static POINT bitArrDiv5[5][5];
static int bitW, bitH;

void ShuffleArray(int size) {

	bitW = WIDTH / size;
	bitH = HEIGHT / size;

	int randomRow = std::rand() % size;
	int randomCol = std::rand() % size;

	int rx = std::rand() % size;
	int ry = std::rand() % size;

	POINT pt;

	for (int j = 0; j < size; j++) {
		for (int i = 0; i < size; i++) {
			bitArrDiv5[i][j] = { bitW * i, bitH * j };
		}
	}


	for (int j = 0; j < size; j++) {
		for (int i = 0; i < size; i++) {
			pt = bitArrDiv5[randomCol][randomRow];
			bitArrDiv5[randomCol][randomRow] = bitArrDiv5[rx][ry];
			bitArrDiv5[rx][ry] = pt;
		}
	}

}



LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	static HDC hdc, memdc;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap;


	static int randomRow;
	static int randomCol;


	static char dividenum;


	switch (message) {
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		dividenum = 'O';
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBitmap);
		BitBlt(hdc, 0, 0, WIDTH, HEIGHT, memdc, 0, 0, SRCCOPY);



		switch (dividenum) {
		case 'T':
			bitW = WIDTH / 3;
			bitH = HEIGHT / 3;
			ShuffleArray(3);
			for (int j = 0; j < 3; j++) {
				for (int i = 0; i < 3; i++) {
					BitBlt(hdc, bitArrDiv5[i][j].x, bitArrDiv5[i][j].y, bitW, bitH, memdc, bitArrDiv5[i][j].x, bitArrDiv5[i][j].y, SRCCOPY);
				}
			}		
			break;
		case 'F':

			for (int j = 0; j < 4; j++) {
				for (int i = 0; i < 4; i++) {
					bitW = WIDTH / 4;
					bitH = HEIGHT / 4;
					randomRow = std::rand() % 4;
					randomCol = std::rand() % 4;
					BitBlt(hdc, bitArrDiv5[i][j].x, bitArrDiv5[i][j].y, bitW, bitH, memdc, bitArrDiv5[randomRow][randomCol].x, bitArrDiv5[randomRow][randomCol].y, SRCCOPY);
				}
			}
			break;
		case 'D':
			for (int j = 0; j < 5; j++) {
				for (int i = 0; i < 5; i++) {
					bitW = WIDTH / 5;
					bitH = HEIGHT / 5;
					randomRow = std::rand() % 5;
					randomCol = std::rand() % 5;
					BitBlt(hdc, bitArrDiv5[i][j].x, bitArrDiv5[i][j].y, bitW, bitH, memdc, bitArrDiv5[randomRow][randomCol].x, bitArrDiv5[randomRow][randomCol].y, SRCCOPY);
				}
			}
			break;
		case'O':
			BitBlt(hdc, 0, 0, WIDTH, HEIGHT, memdc, 0, 0, SRCCOPY);

			break;
		}



		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;


	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_DIVISION3:
			for (int j = 0; j < 3; j++) {
				for (int i = 0; i < 3; i++) {
					int bitW = WIDTH / 3;
					int bitH = HEIGHT / 3;
					bitArrDiv5[i][j] = { bitW*i, bitH*j };
				}
			}

			dividenum = 'T';
			break;


		case ID_DIVISION4:

			for (int j = 0; j < 4; j++) {
				for (int i = 0; i < 4; i++) {
					int bitW = WIDTH / 4;
					int bitH = HEIGHT / 4;
					bitArrDiv5[i][j] = { bitW * i, bitH * j };
				}
			}

			dividenum = 'F';
			break;

		case ID_DIVISION5:

			for (int j = 0; j < 5; j++) {
				for (int i = 0; i < 5; i++) {
					int bitW = WIDTH / 5;
					int bitH = HEIGHT / 5;
					bitArrDiv5[i][j] = { bitW * i, bitH * j };
				}
			}
			dividenum = 'D';
			break;

		case ID_ORIGINAL:
			dividenum = 'O';
			break;

		default:
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

