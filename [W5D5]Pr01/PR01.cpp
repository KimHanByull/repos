#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <time.h>
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

	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("���Ѻ��� 9��° ���� Ŭ���̾�Ʈ ������"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, SW_MAXIMIZE);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void ThreadProc(void* arg) {
	HDC hdc;
	int i;
	int xpos = *((int*)arg);
	srand((unsigned)time(0));
	hdc = GetDC(hwnd);
	SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256,
		rand() % 256,
		rand() % 256))
	);

	for (int i = 0; i <= 10; i++) {
		int num;
		num = rand() % 500;
		Sleep(3000);
		Rectangle(hdc, xpos, 0, xpos + 20, num);
	}
	ReleaseDC(hwnd, hdc);
	return;

}
#define THREAD_NUM 10


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HANDLE hThread[THREAD_NUM];
	static int xpos[THREAD_NUM];
	int i;
	switch (message)
	{
	case WM_CREATE:
		for (i = 0; i < THREAD_NUM; i++)
			xpos[i] = i * 100;
		break;

	case WM_LBUTTONDOWN:
		for (i = 0; i < THREAD_NUM; i++) {
			hThread[i] = (HANDLE)_beginthreadex(
				NULL,
				0,
				(unsigned int(_stdcall*)(void*))ThreadProc,
				(void*)&xpos[i],
				0,
				NULL
			);
			Sleep(2000);
		}
		break;

	case WM_DESTROY:
		for (i = 0; i < THREAD_NUM; i++)
			CloseHandle(hThread[i]);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);

}