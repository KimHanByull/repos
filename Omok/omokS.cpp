// 오목 - 서버 코드

// -------------------------------------------
// 피드백 사항
//1. static SOCKET s, cs --> 구분
// 소켓을 사용하여 통신하는데 보내는 곳과 받는 곳 중에 소켓 이름을 잘못 쓰신 곳이 있습니다.
// 저도 발견하기가 숨은그림 찾기처럼 매우 어려웠네요.
// 2. token = strtok_s((LPSTR)buffer, ",", &context);
// 힌트를 드리면 strtok_s 함수에 넣는 parameter에 문제가 있습니다.
// -------------------------------------------

#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<stdio.h>

#define WM_ASYNC WM_USER+2


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    HWND hwnd;
    MSG msg;
    WNDCLASS WndClass;

    hInst = hInstance;
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = _T("Window Class Name");

    RegisterClass(&WndClass);

    hwnd = CreateWindow(
        _T("Window Class Name"),
        _T("Server Window"),
        WS_OVERLAPPEDWINDOW,
        50,
        50,
        900,
        900,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

#define GAP 40 // 오목판 간격 

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static WSADATA wsadata;
    static SOCKET s, cs;
    static SOCKADDR_IN addr = { 0 }, c_addr;
    static HWND hButton1;
    static bool MyTurn = TRUE;
    static int token2[2];
    static TCHAR msg[10];
    char buffer[100];
    HBRUSH  redBrush, blueBrush;
    static int  mX, mY;
    static int circle[20][20] = { 0, };
    static int recv_circle[20][20] = { 0, };

    int  size, msgLen;
    static TCHAR* context;
    static TCHAR* token;
    int i = 0;

    switch (iMsg)
    {
    case WM_CREATE:
        // 서버용 소켓을 만들고 127.0.0.1, 20에서 클라이언트의 접속을 기다림
        // 클라이언트가 접속하지 않으면 서버 소켓은 계속 기다리고 화면에 윈도우가 나타나지 않음
        WSAStartup(MAKEWORD(2, 2), &wsadata);
        s = socket(AF_INET, SOCK_STREAM, 0);
        addr.sin_family = AF_INET;
        addr.sin_port = 20;
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        bind(s, (LPSOCKADDR)&addr, sizeof(addr));
        // 소켓 s에서 상대방이 접속을 시도하는 이벤트가 발생하면 WM_ASYNC를 hwnd 윈도우에 발생시키기 위해 설정
        WSAAsyncSelect(s, hwnd, WM_ASYNC, FD_ACCEPT);
        if (listen(s, 5) == -1) return 0;
        return 0;
        break;

    case WM_ASYNC:
        switch (lParam)
        {
        case FD_ACCEPT:
            size = sizeof(c_addr);
            cs = accept(s, (LPSOCKADDR)&c_addr, &size);
            WSAAsyncSelect(cs, hwnd, WM_ASYNC, FD_READ);
            break;
        case FD_READ:
            msgLen = recv(cs, buffer, 100, 0);
            buffer[msgLen] = NULL;

            ////if (!_tcscmp((LPTSTR)buffer, _T("-1")))
            if(!strcmp(buffer, "-1"))
            {
                MessageBox(hwnd, _T("아쉽네요. 졌습니다"), _T("다음에 한번 더"), NULL);
                MyTurn = FALSE;
                return 0;
            }
            context = NULL;
            MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, 10);
            token = _tcstok_s(msg, _T(","), &context);       //context에는 분리된 후 남은 문자열이 들어간다.
            while (token != NULL)
            {
                token2[i++] = _ttoi(token);
                token = _tcstok_s(NULL, _T(","), &context);
            }
            recv_circle[token2[0]][token2[1]] = 1;
            MyTurn = TRUE;
            InvalidateRgn(hwnd, NULL, TRUE);
            break;
        default:
            break;
        }
        break;
    case WM_LBUTTONDOWN:
        if (MyTurn)
        {
            int sum[8] = { 0, };
            mX = LOWORD(lParam);
            mY = HIWORD(lParam);
            if ((mY / GAP) < 20 && (mX / GAP) < 20 && circle[mY / GAP][mX / GAP] == 0 && recv_circle[mY / GAP][mX / GAP] == 0)
            { // 
                circle[mY / GAP][mX / GAP] = 1; // 원을 그려넣으면 1

                sprintf_s(buffer, "%d,%d", mY / GAP, mX / GAP);
                send(cs, buffer, 10, 0);
                MyTurn = FALSE;
                

                
                for (int i = 1; i <= 4; i++)
                {
                    if (mY / GAP - i >= 0 && mY / GAP + i < 20)       // 방금 놓은 좌표를 기준으로 위, 아래 확인
                    {
                        sum[0] += circle[mY / GAP - i][mX / GAP];
                        sum[1] += circle[mY / GAP + i][mX / GAP];
                    }
                    if (mY / GAP - i >= 0 && mY / GAP + i < 20 && mX / GAP - i >= 0 && mX / GAP + i < 20)        //오른쪽위,왼쪽아래
                    {
                        sum[2] += circle[mY / GAP - i][mX / GAP + i];
                        sum[3] += circle[mY / GAP + i][mX / GAP - i];
                    }
                    if (mX / GAP - i >= 0 && mX / GAP + i < 20)        //오른쪽,왼쪽
                    {
                        sum[4] += circle[mY / GAP][mX / GAP + i];
                        sum[5] += circle[mY / GAP][mX / GAP - i];
                    }
                    if (mY / GAP - i >= 0 && mY / GAP + i < 20 && mX / GAP - i >= 0 && mX / GAP + i < 20)        //오른쪽아래,왼쪽위
                    {
                        sum[6] += circle[mY / GAP + i][mX / GAP + i];
                        sum[7] += circle[mY / GAP - i][mX / GAP - i];
                    }
                }

            }
            InvalidateRgn(hwnd, NULL, TRUE);
            // 오목인지 확인
            if (sum[0] + sum[1] >= 4 || sum[2] + sum[3] >= 4 || sum[4] + sum[5] >= 4 || sum[6] + sum[7] >= 4)
            {
                MessageBox(hwnd, _T("축하 합니다. 당신이 이겼습니다"), _T("승리 축하"), NULL);
                MyTurn = FALSE;
                sprintf_s(buffer, "-1");
                send(cs, buffer, 10, 0);
            }
        }
        break;
    case WM_PAINT:
        // 오목판 그리기
        hdc = BeginPaint(hwnd, &ps);
        for (int i = 0; i < 21; i++)            
        {
            MoveToEx(hdc, 0, i * GAP, NULL);
            LineTo(hdc, GAP * 20, i * GAP);
        }
        for (int i = 0; i < 21; i++)
        {
            MoveToEx(hdc, i * GAP, 0, NULL);
            LineTo(hdc, i * GAP, GAP * 20);
        }                                    

        // server의 바둑돌 표시
        redBrush = CreateSolidBrush(RGB(255, 0, 0));
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                SelectObject(hdc, redBrush);
                if (circle[i][j])
                    Ellipse(hdc, GAP * j, GAP * i, GAP * j + GAP, GAP * i + GAP);
            }
        }

        // client의 바둑돌 표시
        blueBrush = CreateSolidBrush(RGB(0, 0, 255));
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                SelectObject(hdc, blueBrush);
                if (recv_circle[i][j])
                    Ellipse(hdc, GAP * j, GAP * i, GAP * j + GAP, GAP * i + GAP);
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
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
