
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>

#define WM_ASYNC WM_USER+2


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
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("HelloClass");
	if (!RegisterClass(&wndclass)) return 1;


	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("김한별의 연습문제 9-1번째 서버 윈도우"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;
	static WSADATA wsadata;
	static SOCKET s, cs;
	static TCHAR msg[10] = { '0', };
	static HWND hButton1;

	static SOCKADDR_IN addr = { 0 }, c_addr;
	static int recv_x1 = 0, recv_x2 = 0, recv_y1 = 0, recv_y2 = 0;
	static int recv_shape = 0;
	static int pos_y = 0;
	static int mx = 0, my = 0;

	static bool Drag = false;
	static int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	static int shape = 0;
	static int token2[2];

	HPEN blackPen;
	static int mX, mY;
	static int circle[20][20] = { 0, };			//클릭한 원 좌표
	static int recv_circle[20][20] = { 0, };	//클라이언트에서 클릭한 원 좌표

	int size, msgLen;
	char buffer[100] = { '0', };
	char *context;
	char *token;
	int i = 0;

	switch (message) {
	case WM_CREATE:
		WSAStartup(MAKEWORD(2, 2), &wsadata);
		s = socket(AF_INET, SOCK_STREAM, 0);
		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		bind(s, (LPSOCKADDR)&addr, sizeof(addr));
		WSAAsyncSelect(s, hwnd, WM_ASYNC, FD_ACCEPT);
		if (listen(s, 5) == -1) return 0;
		break;

	case WM_ASYNC:
		switch (lParam) {
		case FD_ACCEPT:
			size = sizeof(c_addr);
			cs = accept(s, (LPSOCKADDR)&c_addr, &size);
			WSAAsyncSelect(cs, hwnd, WM_ASYNC, FD_READ);
			break;
		case FD_READ:
			recv(cs, buffer, 100, 0);
			strcpy_s(msg, buffer);
			context = NULL;
			token = strtok_s(msg, ",", &context);

			while (token != NULL) {
				token2[i++] = atoi(token);
				token = strtok_s(NULL, ",", &context);
				pos_y += 20;
			}
			recv_circle[token2[0]][token2[1]] = 1;
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		default:
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		mX = LOWORD(lParam);
		mY = LOWORD(lParam);
		if ((mY / 30) < 20 && (mX / 30) < 20) {
			circle[mY / 30][mX / 30] = 1;
			_stprintf_s(buffer, _T("%d%d"), mY / 30, mX / 30);
			send(cs, (LPSTR)buffer, 10, 0);
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_MOUSEMOVE:
		if (Drag) {
			x2 = LOWORD(lParam);
			y2 = HIWORD(lParam);
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
	case WM_LBUTTONUP:
		Drag = false;
		x2 = LOWORD(lParam);
		y2 = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (i = 0; i <= 20; i++) {
			MoveToEx(hdc, 0, i * 30, NULL);
			LineTo(hdc, 600, i * 30);
		}for (i = 0; i <= 20; i++) {
			MoveToEx(hdc, i * 30, 0, NULL);
			LineTo(hdc, i * 30, 600);
		}
		blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		for (i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				SelectObject(hdc, blackPen);
				if (circle[i][j])
					Ellipse(hdc, 30 * j, 30 * i, 30 * j + 30, 30 * i + 30);
			}
		}
		for (i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				SelectObject(hdc, blackPen);
				if (recv_circle[j][i])
					Ellipse(hdc, 30 * j, 30 * i, 30 * j + 30, 30 * i + 30);
			}
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		closesocket(s);
		WSACleanup();
		PostQuitMessage(0);
		break;
	
	}

	return DefWindowProc(hwnd, message, wParam, lParam);

}