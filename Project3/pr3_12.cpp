
#include <windows.h>
#include <iostream>
#include <math.h>
#include <windowsx.h>

#define BSIZE 100

using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int clickedRect = -1; // 클릭된 Rectangle의 인덱스


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
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("HelloClass");
	if (!RegisterClass(&wndclass)) return 1;


	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("김한별의 세 번째 윈도우"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, SW_MAXIMIZE);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

double LengthPts(int x1, int y1, int x2, int y2) {
	return(sqrt((float)((x2 - x1)*(x2 - x1) + (y2 - y1) * (y2 - y1))));
}



BOOL InRectangle(int x, int y, int mx, int my) {
	
	if (LengthPts(x, y, mx, my) < BSIZE) 
		return TRUE;
	else
		return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	POINT pt;

	static BOOL Selection;
	int	mx, my;

	RECT rectArr[32];

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 8; i++) {
				rectArr[i + (i * j)] = { i * BSIZE, 0, (i * BSIZE) + BSIZE, BSIZE };
		}
	}


	switch (message) {
	case WM_CREATE:
	
		
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 8; i++) {
				/*if (j == 0) {
					Rectangle(hdc, i * BSIZE, 0, (i * BSIZE) + BSIZE, BSIZE);
				}*/
				Rectangle(hdc, i * BSIZE, j* BSIZE, (i * BSIZE) + BSIZE, (j+1)* BSIZE);
				
			}
		}

            // 클릭된 Rectangle에 원 그리기
            if (clickedRect != -1) {
                Ellipse(hdc, rectArr[clickedRect].left, rectArr[clickedRect].top,
                    rectArr[clickedRect].right, rectArr[clickedRect].bottom);
            }

            EndPaint(hwnd, &ps);
            return 0;

        case WM_LBUTTONDOWN:
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);

			for (int i = 0; i < 8; i++) {
				if (InRectangle(pt.x, pt.y, rectArr[i].left + BSIZE / 2, rectArr[i].top + BSIZE / 2)) {
					clickedRect = i;
					// 전체 윈도우를 다시 그리도록 강제로 WM_PAINT 메시지 전송
					InvalidateRect(hwnd, NULL, TRUE);
					break;
				}
			}
			if (InRectangle(pt.x, pt.y, rectArr[5].left, rectArr[5].top)) 
				Selection = TRUE;
			InvalidateRgn(hwnd, NULL, TRUE);
			
		
			return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);

}