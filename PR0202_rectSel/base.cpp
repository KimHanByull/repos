#include <windows.h>
#include <iostream>
#include <math.h>
#include <windowsx.h>

#define BSIZE 100

using namespace std;

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    POINT pt{};

    HPEN redPen, blackPen, bluePen;

    static int mx, my;
    static RECT rectArr[4][8];
    static int circlePaint[4][8];
    static int clickCount = 0;


    static int clickedRectX;
    static int clickedRectY;

    switch (message) {
    case WM_CREATE:

        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 8; i++) {
                rectArr[i][j] = { i * 100, j * 100, (i + 1) * 100, (j + 1) * 100 };
                circlePaint[i][j] = 0;
            }
        }

        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 8; i++) {
                Rectangle(hdc, i * BSIZE, j * BSIZE, (i * BSIZE) + BSIZE, (j + 1) * BSIZE);
            }
        }
        EndPaint(hwnd, &ps);
        return 0;

    case WM_LBUTTONDOWN:
        hdc = GetDC(hwnd);
        mx = LOWORD(lParam);
        my = HIWORD(lParam);

        //rectArr[my / 100][mx / 100] = (mx / 100) * 100;

        clickedRectX = mx / BSIZE;
        clickedRectY = my / BSIZE;

        // 사각형 내부에만 원 그리기
        if (clickedRectX >= 0 && clickedRectX < 8 && clickedRectY >= 0 && clickedRectY < 4) {
            rectArr[clickedRectY][clickedRectX] = clickedRectX * BSIZE;
            rectArr[clickedRectY][clickedRectX] = clickedRectY * BSIZE;
        }

        if (clickCount == 0) {
            circlePaint[my / 100][mx / 100] = 1;
            clickCount = 1;
        }
        else {
            circlePaint[my / 100][mx / 100] = 2;
            clickCount = 0;
        }


        blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
        bluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 8; j++) {
                if (circlePaint[i][j] == 1)
                    SelectObject(hdc, redPen);
                else if (circlePaint[i][j] == 2)
                    SelectObject(hdc, bluePen);
                Ellipse(hdc, rectArrX[i][j], rectArrY[i][j], rectArrX[i][j] + 100, rectArrY[i][j] + 100);
            }
        }

        // 생성한 펜 해제
        DeleteObject(redPen);
        DeleteObject(bluePen);
        DeleteObject(blackPen);

        ReleaseDC(hwnd, hdc);

        //InvalidateRgn(hwnd, NULL, TRUE);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}
