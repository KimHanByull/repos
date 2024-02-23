//#include <windows.h>
//#include <tchar.h>
//#include <windowsx.h>
//#include <stdio.h>
//#include <stdbool.h>
//
//// 상수 정의
//#define WIN_CONDITION 5         // 연속된 원의 갯수 파악용
//#define WM_ASYNC WM_USER + 2
//#define BSIZE 30                // 바둑판 한 칸의 크기
//
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//bool CheckWinCondition(int x, int y, int cliPaint[20][20]);
//void EndGame(HWND hwnd);
//
//bool isClientTurn = true;
//bool isGameOver = false;
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
//
//	WNDCLASS wndclass;
//	HWND hwnd;
//	MSG msg;
//
//	wndclass.style = CS_HREDRAW | CS_VREDRAW;
//	wndclass.lpfnWndProc = WndProc;
//	wndclass.cbClsExtra = 0;
//	wndclass.cbWndExtra = 0;
//	wndclass.hInstance = hInstance;
//	wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION);
//	wndclass.hCursor = LoadCursor(NULL, IDC_IBEAM);
//	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//	wndclass.lpszMenuName = NULL;
//	wndclass.lpszClassName = TEXT("HelloClass");
//	if (!RegisterClass(&wndclass)) return 1;
//
//	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("김한별의 9번째 오목 클라이언트 윈도우"),
//		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
//		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
//	ShowWindow(hwnd, nCmdShow);
//
//	while (GetMessage(&msg, NULL, 0, 0) > 0) {
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//
//		if (isGameOver) {
//			// 게임 종료 여부에 따라 클라이언트의 턴 설정
//			isClientTurn = false;
//		}
//	}
//
//	return msg.wParam;
//}
//
//
//
//static int x;
//static int y;
//static SOCKET cs, s;
//
//LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
//
//	HDC hdc;
//	PAINTSTRUCT ps;
//	static WSADATA wsadata;
//
//	static SOCKADDR_IN addr = { 0 };
//	static TCHAR msg[1000];
//	int msgLen;
//	char buffer[100];
//	static RECT rectArr[20][20];
//	static int cliPaint[20][20];
//	static int serverPaint[20][20];
//	static int clickedRectX, clickedRectY;
//
//	HPEN RPen, BPen;
//	BPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
//	RPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
//
//	switch (message) {
//	case WM_CREATE:
//		WSAStartup(MAKEWORD(2, 2), &wsadata);
//		s = socket(AF_INET, SOCK_STREAM, 0);
//		addr.sin_family = AF_INET;
//		addr.sin_port = 20;
//		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//		WSAAsyncSelect(s, hwnd, WM_ASYNC, FD_READ);
//		if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == -1) return 0;
//		break;
//	case WM_ASYNC:
//		switch (lParam) {
//		case FD_READ:
//			msgLen = recv(s, buffer, 100, 0);
//			buffer[msgLen] = NULL;
//#ifdef _UNICODE
//			msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer),
//				NULL, NULL);
//			MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
//			buffer[msgLen] = NULL;
//#else
//			strcpy_s(msg, buffer);
//#endif // _UNICODE
//
//			// 서버에서 전송한 데이터 처리
//			if (strncmp(buffer, "CLICKED:", 8) == 0) {
//				// 예시: CLICKED:i,j
//				int clicked_i, clicked_j;
//				sscanf_s(buffer + 8, "%d,%d", &clicked_i, &clicked_j);
//
//				// 서버에서 전송한 좌표에 원 출력
//				if (clicked_i >= 0 && clicked_i < 20 && clicked_j >= 0 && clicked_j < 20) {
//					serverPaint[clicked_i][clicked_j] = 1;
//					InvalidateRect(hwnd, &rectArr[clicked_i][clicked_j], TRUE);
//				}
//
//				// 클라이언트의 턴으로 변경
//				isClientTurn = true;
//			}
//
//			if (strncmp(buffer, "SERVER_WON", strlen("SERVER_WON")) == 0) {
//				MessageBox(hwnd, TEXT("패배했습니다!"), TEXT("게임 종료"), MB_OK | MB_ICONINFORMATION);
//				// 게임 종료 또는 초기화 등 추가적인 로직 수행
//				isClientTurn = false;
//			}
//
//			InvalidateRgn(hwnd, NULL, TRUE);
//			break;
//		default:
//			break;
//		}
//		break;
//	case WM_PAINT:
//		hdc = BeginPaint(hwnd, &ps);
//		for (int j = 0; j < 20; j++) {
//			for (int i = 0; i < 20; i++) {
//				rectArr[i][j] = { i * BSIZE, j * BSIZE, (i + 1) * BSIZE, (j + 1) * BSIZE };
//				Rectangle(hdc, i * BSIZE, j * BSIZE, (i * BSIZE) + BSIZE, (j + 1) * BSIZE);
//
//				if (serverPaint[i][j] == 1) {
//					// 서버가 그린 원 (빨간색)
//					HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
//					HGDIOBJ hOldPen = SelectObject(hdc, hPen);
//
//					Ellipse(hdc, rectArr[i][j].left, rectArr[i][j].top, rectArr[i][j].right, rectArr[i][j].bottom);
//
//					SelectObject(hdc, hOldPen);
//					DeleteObject(hPen);
//				}
//				else if (cliPaint[i][j] == 1) {
//					// 클라이언트가 그린 원 (파란색)
//					HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
//					HGDIOBJ hOldPen = SelectObject(hdc, hPen);
//
//					Ellipse(hdc, rectArr[i][j].left, rectArr[i][j].top, rectArr[i][j].right, rectArr[i][j].bottom);
//
//					SelectObject(hdc, hOldPen);
//					DeleteObject(hPen);
//				}
//
//
//			}
//		}
//		EndPaint(hwnd, &ps);
//		break;
//
//	case WM_LBUTTONDOWN:
//		x = GET_X_LPARAM(lParam);
//		y = GET_Y_LPARAM(lParam);
//		for (int j = 0; j < 20; j++) {
//			for (int i = 0; i < 20; i++) {
//				if (x >= rectArr[i][j].left && x <= rectArr[i][j].right &&
//					y >= rectArr[i][j].top && y <= rectArr[i][j].bottom) {
//					clickedRectX = i;
//					clickedRectY = j;
//
//					if (cliPaint[i][j] == 0 && isClientTurn) {
//						cliPaint[i][j] = 1;
//
//						InvalidateRect(hwnd, &rectArr[i][j], TRUE); // 해당 사각형 영역만 갱신
//						// 해당 위치 정보를 서버로 전송
//						char data[100];
//						sprintf_s(data, sizeof(data), "CLICKED:%d,%d", i, j);
//						send(s, data, strlen(data), 0);
//
//						// 서버의 턴으로 변경
//						isClientTurn = false;
//
//						if (CheckWinCondition(i, j, cliPaint)) {
//							MessageBox(hwnd, TEXT("이겼습니다!"), TEXT("게임 종료"), MB_OK | MB_ICONINFORMATION);
//							// 게임 종료 또는 초기화 등 추가적인 로직 수행
//							EndGame(hwnd);
//
//						}
//
//					}
//				}
//			}
//		}
//		break;
//
//	case WM_DESTROY:
//		closesocket(s);
//		WSACleanup();
//		PostQuitMessage(0);
//		break;
//	}
//
//	return DefWindowProc(hwnd, message, wParam, lParam);
//}
//
//// 게임 종료 함수
//void EndGame(HWND hwnd) {
//	//MessageBox(hwnd, TEXT("이겼습니다!"), TEXT("게임 종료"), MB_OK | MB_ICONINFORMATION);
//	isGameOver = true;
//
//	send(s, "CLIENT_WON", strlen("CLIENT_WON"), 0);
//}
//
//// 승리 조건 확인 함수
//bool CheckWinCondition(int x, int y, int cliPaint[20][20]) {
//	// 상하 확인
//	int count = 1;
//	for (int i = 1; i < WIN_CONDITION; i++) {
//		if (y - i >= 0 && cliPaint[x][y - i] == 1) {
//			count++;
//		}
//		else {
//			break;
//		}
//	}
//
//	for (int i = 1; i < WIN_CONDITION; i++) {
//		if (y + i < 20 && cliPaint[x][y + i] == 1) {
//			count++;
//		}
//		else {
//			break;
//		}
//	}
//
//	if (count >= WIN_CONDITION) {
//		return true;
//	}
//
//	// 좌우 확인
//	count = 1;
//	for (int i = 1; i < WIN_CONDITION; i++) {
//		if (x - i >= 0 && cliPaint[x - i][y] == 1) {
//			count++;
//		}
//		else {
//			break;
//		}
//	}
//
//	for (int i = 1; i < WIN_CONDITION; i++) {
//		if (x + i < 20 && cliPaint[x + i][y] == 1) {
//			count++;
//		}
//		else {
//			break;
//		}
//	}
//
//	if (count >= WIN_CONDITION) {
//		return true;
//	}
//
//	// 대각선 (왼쪽 위 오른쪽 아래)
//	count = 1;
//	for (int i = 1; i < WIN_CONDITION; i++) {
//		if (x - i >= 0 && y - i >= 0 && cliPaint[x - i][y - i] == 1) {
//			count++;
//		}
//		else {
//			break;
//		}
//	}
//
//	for (int i = 1; i < WIN_CONDITION; i++) {
//		if (x + i < 20 && y + i < 20 && cliPaint[x + i][y + i] == 1) {
//			count++;
//		}
//		else {
//			break;
//		}
//	}
//
//	if (count >= WIN_CONDITION) {
//		return true;
//	}
//
//	// 대각선 (오른쪽 위 왼쪽 아래)
//	count = 1;
//	for (int i = 1; i < WIN_CONDITION; i++) {
//		if (x - i >= 0 && y + i < 20 && cliPaint[x - i][y + i] == 1) {
//			count++;
//		}
//		else {
//			break;
//		}
//	}
//
//	for (int i = 1; i < WIN_CONDITION; i++) {
//		if (x + i < 20 && y - i >= 0 && cliPaint[x + i][y - i] == 1) {
//			count++;
//		}
//		else {
//			break;
//		}
//	}
//
//	if (count >= WIN_CONDITION) {
//		return true;
//	}
//
//	return false;
//}
