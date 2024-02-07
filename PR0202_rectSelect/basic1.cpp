#include <windows.h>
#include <iostream>
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

    HPEN redPen, bluePen;

    static int mx, my;
    static RECT rectArr[8][4];
    static int circlePaint[8][4];
    static int clickCount = 0;

    static int clickedRectX;
    static int clickedRectY;

    switch (message) {
    case WM_CREATE:

        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 8; i++) {
                rectArr[i][j] = { i * BSIZE, j * BSIZE, (i + 1) * BSIZE, (j + 1) * BSIZE };
                circlePaint[i][j] = 0;
            }
        }

        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 8; i++) {
                Rectangle(hdc, i * BSIZE, j * BSIZE, (i * BSIZE) + BSIZE, (j + 1) * BSIZE);
            }
        }
        EndPaint(hwnd, &ps);
        break;

    case WM_LBUTTONDOWN:
        hdc = GetDC(hwnd);
        mx = LOWORD(lParam);
        my = HIWORD(lParam);

        clickedRectX = mx / BSIZE;
        clickedRectY = my / BSIZE;

        // 마우스 클릭한 좌표가 표 안에 있고, 해당 좌표에 원이 그려져있지 않은 경우에만 처리
        if (clickedRectX >= 0 && clickedRectX < 8 && clickedRectY >= 0 && clickedRectY < 4 && circlePaint[clickedRectX][clickedRectY] == 0) {
            clickCount++;

            if (clickCount % 2 == 1)
                circlePaint[clickedRectX][clickedRectY] = 1;
            else
                circlePaint[clickedRectX][clickedRectY] = 2;

            redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
            bluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

            for (int j = 0; j < 4; j++) {
                for (int i = 0; i < 8; i++) {
                    if (circlePaint[i][j] == 1)
                        SelectObject(hdc, redPen);
                    else if (circlePaint[i][j] == 2)
                        SelectObject(hdc, bluePen);

                    // 사각형 내부에만 원 그리기
                    if (i == clickedRectX && j == clickedRectY)
                        Ellipse(hdc, rectArr[i][j].left, rectArr[i][j].top, rectArr[i][j].right, rectArr[i][j].bottom);
                }
            }

            // 생성한 펜 해제
            DeleteObject(redPen);
            DeleteObject(bluePen);
        }

        ReleaseDC(hwnd, hdc);

        //InvalidateRgn(hwnd, NULL, TRUE);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}
