#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <windowsx.h>
#include <stdbool.h>

// 상수 정의
#define WIN_CONDITION 5         //연속된 원의 갯수 파악용
#define BSIZE 30
#define WM_ASYNC WM_USER + 2

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool CheckWinCondition(int x, int y, int serverPaint[20][20]);
void EndGame(HWND hwnd);

bool isServerTurn = true;  // 초기에 서버가 먼저 턴을 갖음
bool isGameOver = false;
bool isGameLocked = false;

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

	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("김한별의 9번째 오목 서버 윈도우"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
		if (isGameOver) {
			// 게임 종료 여부에 따라 클라이언트의 턴 설정
			isServerTurn = false;
		}
	}

	return msg.wParam;
}


static int x;
static int y;
static SOCKET cs, s;
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;
	static WSADATA wsadata;

	static SOCKADDR_IN c_addr, addr = { 0 };
	static TCHAR msg[1000];
	int msgLen, size;
	char buffer[100];
	static RECT rectArr[20][20];
	static int serverPaint[20][20];
	static int cliPaint[20][20];
	static int clickedRectX, clickedRectY;

	HPEN RPen, BPen;
	BPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	RPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

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
			msgLen = recv(cs, buffer, 100, 0);
			buffer[msgLen] = NULL;
#ifdef _UNICODE
			msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer),
				NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
			buffer[msgLen] = NULL;
#else
			strcpy_s(msg, buffer);
#endif // _UNICODE

			// 클라이언트에서 전송한 데이터 처리
			if (strncmp(buffer, "CLICKED:", 8) == 0 && isServerTurn) {
				int clicked_i, clicked_j;
				sscanf_s(buffer + 8, "%d,%d", &clicked_i, &clicked_j);

				// 클라이언트에서 전송한 좌표에 원 출력
				if (clicked_i >= 0 && clicked_i < 20 && clicked_j >= 0 && clicked_j < 20) {
					cliPaint[clicked_i][clicked_j] = 1;
					InvalidateRect(hwnd, &rectArr[clicked_i][clicked_j], TRUE);
				}

				// 클라이언트의 턴으로 변경
				isServerTurn = false;
			}
			if (strncmp(buffer, "CLIENT_WON", strlen("CLIENT_WON")) == 0) {
				MessageBox(hwnd, TEXT("패배했습니다!"), TEXT("게임 종료"), MB_OK | MB_ICONINFORMATION);
				// 게임 종료 또는 초기화 등 추가적인 로직 수행
				EndGame(hwnd);
				DestroyWindow(hwnd);
			}

			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		default:
			break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int j = 0; j < 20; j++) {
			for (int i = 0; i < 20; i++) {
				rectArr[i][j] = { i * BSIZE, j * BSIZE, (i + 1) * BSIZE, (j + 1) * BSIZE };
				Rectangle(hdc, i * BSIZE, j * BSIZE, (i * BSIZE) + BSIZE, (j + 1) * BSIZE);

				if (serverPaint[i][j] == 1) {
					// 서버가 그린 원 (빨간색)
					HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
					HGDIOBJ hOldPen = SelectObject(hdc, hPen);

					Ellipse(hdc, rectArr[i][j].left, rectArr[i][j].top, rectArr[i][j].right, rectArr[i][j].bottom);

					SelectObject(hdc, hOldPen);
					DeleteObject(hPen);
				}
				else if (cliPaint[i][j] == 1) {
					// 클라이언트가 그린 원 (파란색)
					HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
					HGDIOBJ hOldPen = SelectObject(hdc, hPen);

					Ellipse(hdc, rectArr[i][j].left, rectArr[i][j].top, rectArr[i][j].right, rectArr[i][j].bottom);

					SelectObject(hdc, hOldPen);
					DeleteObject(hPen);
				}
			}
		}

		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		if (!isGameLocked) { // 입력 차단 여부 확인
			x = GET_X_LPARAM(lParam);
			y = GET_Y_LPARAM(lParam);
			for (int j = 0; j < 20; j++) {
				for (int i = 0; i < 20; i++) {
					if (x >= rectArr[i][j].left && x <= rectArr[i][j].right &&
						y >= rectArr[i][j].top && y <= rectArr[i][j].bottom) {
						clickedRectX = i;
						clickedRectY = j;

						if (serverPaint[i][j] == 0 && !isServerTurn) {
							serverPaint[i][j] = 1;

							InvalidateRect(hwnd, &rectArr[i][j], TRUE); // 해당 사각형 영역만 갱신
							// 해당 위치 정보를 클라이언트로 전송
							char data[50];
							sprintf_s(data, "CLICKED:%d,%d", i, j);
							send(cs, data, strlen(data), 0);

							// 서버의 턴으로 변경
							isServerTurn = true;

							if (CheckWinCondition(i, j, serverPaint)) {
								MessageBox(hwnd, TEXT("이겼습니다!"), TEXT("게임 종료"), MB_OK | MB_ICONINFORMATION);
								// 게임 종료 또는 초기화 등 추가적인 로직 수행
								EndGame(hwnd);
								DestroyWindow(hwnd);
							}
						}
					}
				}
			}
		}
		break;

	case WM_DESTROY:
		closesocket(s);
		WSACleanup();
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

//게임 종료 함수
void EndGame(HWND hwnd) {
	//MessageBox(hwnd, TEXT("이겼습니다!"), TEXT("게임 종료"), MB_OK | MB_ICONINFORMATION);
	isGameOver = true;
	send(cs, "SERVER_WON", strlen("SERVER_WON"), 0);
	isGameLocked = true; // 게임 종료 시 입력 차단
}


// 승리 조건 확인 함수
bool CheckWinCondition(int x, int y, int serverPaint[20][20]) {
	// 상하 확인
	int count = 1;
	for (int i = 1; i < WIN_CONDITION; i++) {
		if (y - i >= 0 && serverPaint[x][y - i] == 1) {
			count++;
		}
		else {
			break;
		}
	}

	for (int i = 1; i < WIN_CONDITION; i++) {
		if (y + i < 20 && serverPaint[x][y + i] == 1) {
			count++;
		}
		else {
			break;
		}
	}

	if (count >= WIN_CONDITION) {
		return true;
	}

	// 좌우 확인
	count = 1;
	for (int i = 1; i < WIN_CONDITION; i++) {
		if (x - i >= 0 && serverPaint[x - i][y] == 1) {
			count++;
		}
		else {
			break;
		}
	}

	for (int i = 1; i < WIN_CONDITION; i++) {
		if (x + i < 20 && serverPaint[x + i][y] == 1) {
			count++;
		}
		else {
			break;
		}
	}

	if (count >= WIN_CONDITION) {
		return true;
	}

	// 대각선 (왼쪽 위 오른쪽 아래)
	count = 1;
	for (int i = 1; i < WIN_CONDITION; i++) {
		if (x - i >= 0 && y - i >= 0 && serverPaint[x - i][y - i] == 1) {
			count++;
		}
		else {
			break;
		}
	}

	for (int i = 1; i < WIN_CONDITION; i++) {
		if (x + i < 20 && y + i < 20 && serverPaint[x + i][y + i] == 1) {
			count++;
		}
		else {
			break;
		}
	}

	if (count >= WIN_CONDITION) {
		return true;
	}

	// 대각선 (오른쪽 위 왼쪽 아래)
	count = 1;
	for (int i = 1; i < WIN_CONDITION; i++) {
		if (x - i >= 0 && y + i < 20 && serverPaint[x - i][y + i] == 1) {
			count++;
		}
		else {
			break;
		}
	}

	for (int i = 1; i < WIN_CONDITION; i++) {
		if (x + i < 20 && y - i >= 0 && serverPaint[x + i][y - i] == 1) {
			count++;
		}
		else {
			break;
		}
	}

	if (count >= WIN_CONDITION) {
		return true;
	}

	return false;
}
