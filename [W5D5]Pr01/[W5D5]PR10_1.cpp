#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <time.h>
#include <windowsx.h>
HWND hwnd;

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

	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("김한별의 10번째 윈도우"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, SW_MAXIMIZE);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

#define THREAD_NUM 50
int XPOS, YPOS;
void ThreadProc();

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;
	static HANDLE hThread[THREAD_NUM];
	static int count = 0;
	static RECT rt;

	switch (message)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rt);
		break;

	case WM_LBUTTONDOWN:
		if (count < THREAD_NUM) {

			//임의의 값에 생성
			XPOS = rand() % rt.right;
			YPOS = rand() % rt.bottom;

			/*XPOS = GET_X_LPARAM(lParam);
			YPOS = GET_Y_LPARAM(lParam);*/
			hThread[count] = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(_stdcall*)(void*))ThreadProc,
				NULL, 0, NULL);
			count++;
		}
		break;
	case WM_DESTROY:
		for (int i = 0; i < THREAD_NUM; i++)
			CloseHandle(hThread[i]);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);

}

void ThreadProc() {

	int xPos, yPos;
	HDC hdc;
	srand((unsigned)time(0));
	hdc = GetDC(hwnd);

	xPos = XPOS;
	yPos = YPOS;

	//원을 새로 그릴 때 원 뒤에 검정색 배경이 생기는 문제 해결하기 위하여 배경 다시 색칠
	PatBlt(hdc, xPos - 10, yPos - 10, 20, 20, WHITENESS);

	SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256,
		rand() % 256,
		rand() % 256))
	);
	Ellipse(hdc, xPos - 10, yPos - 10, xPos + 10, yPos + 10);

	for (int i = 0; i <= 10; i++) {

		Sleep(1000);
		Ellipse(hdc, xPos - 10, yPos - 10, xPos + 10, yPos + 10);
		SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256,
			rand() % 256,
			rand() % 256))
		);
	}
	ReleaseDC(hwnd, hdc);
	return;

}
