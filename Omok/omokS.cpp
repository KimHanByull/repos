// ���� - ���� �ڵ�

// -------------------------------------------
// �ǵ�� ����
//1. static SOCKET s, cs --> ����
// ������ ����Ͽ� ����ϴµ� ������ ���� �޴� �� �߿� ���� �̸��� �߸� ���� ���� �ֽ��ϴ�.
// ���� �߰��ϱⰡ �����׸� ã��ó�� �ſ� ������׿�.
// 2. token = strtok_s((LPSTR)buffer, ",", &context);
// ��Ʈ�� �帮�� strtok_s �Լ��� �ִ� parameter�� ������ �ֽ��ϴ�.
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

#define GAP 40 // ������ ���� 

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
        // ������ ������ ����� 127.0.0.1, 20���� Ŭ���̾�Ʈ�� ������ ��ٸ�
        // Ŭ���̾�Ʈ�� �������� ������ ���� ������ ��� ��ٸ��� ȭ�鿡 �����찡 ��Ÿ���� ����
        WSAStartup(MAKEWORD(2, 2), &wsadata);
        s = socket(AF_INET, SOCK_STREAM, 0);
        addr.sin_family = AF_INET;
        addr.sin_port = 20;
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        bind(s, (LPSOCKADDR)&addr, sizeof(addr));
        // ���� s���� ������ ������ �õ��ϴ� �̺�Ʈ�� �߻��ϸ� WM_ASYNC�� hwnd �����쿡 �߻���Ű�� ���� ����
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
                MessageBox(hwnd, _T("�ƽ��׿�. �����ϴ�"), _T("������ �ѹ� ��"), NULL);
                MyTurn = FALSE;
                return 0;
            }
            context = NULL;
            MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, 10);
            token = _tcstok_s(msg, _T(","), &context);       //context���� �и��� �� ���� ���ڿ��� ����.
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
                circle[mY / GAP][mX / GAP] = 1; // ���� �׷������� 1

                sprintf_s(buffer, "%d,%d", mY / GAP, mX / GAP);
                send(cs, buffer, 10, 0);
                MyTurn = FALSE;
                

                
                for (int i = 1; i <= 4; i++)
                {
                    if (mY / GAP - i >= 0 && mY / GAP + i < 20)       // ��� ���� ��ǥ�� �������� ��, �Ʒ� Ȯ��
                    {
                        sum[0] += circle[mY / GAP - i][mX / GAP];
                        sum[1] += circle[mY / GAP + i][mX / GAP];
                    }
                    if (mY / GAP - i >= 0 && mY / GAP + i < 20 && mX / GAP - i >= 0 && mX / GAP + i < 20)        //��������,���ʾƷ�
                    {
                        sum[2] += circle[mY / GAP - i][mX / GAP + i];
                        sum[3] += circle[mY / GAP + i][mX / GAP - i];
                    }
                    if (mX / GAP - i >= 0 && mX / GAP + i < 20)        //������,����
                    {
                        sum[4] += circle[mY / GAP][mX / GAP + i];
                        sum[5] += circle[mY / GAP][mX / GAP - i];
                    }
                    if (mY / GAP - i >= 0 && mY / GAP + i < 20 && mX / GAP - i >= 0 && mX / GAP + i < 20)        //�����ʾƷ�,������
                    {
                        sum[6] += circle[mY / GAP + i][mX / GAP + i];
                        sum[7] += circle[mY / GAP - i][mX / GAP - i];
                    }
                }

            }
            InvalidateRgn(hwnd, NULL, TRUE);
            // �������� Ȯ��
            if (sum[0] + sum[1] >= 4 || sum[2] + sum[3] >= 4 || sum[4] + sum[5] >= 4 || sum[6] + sum[7] >= 4)
            {
                MessageBox(hwnd, _T("���� �մϴ�. ����� �̰���ϴ�"), _T("�¸� ����"), NULL);
                MyTurn = FALSE;
                sprintf_s(buffer, "-1");
                send(cs, buffer, 10, 0);
            }
        }
        break;
    case WM_PAINT:
        // ������ �׸���
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

        // server�� �ٵϵ� ǥ��
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

        // client�� �ٵϵ� ǥ��
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
