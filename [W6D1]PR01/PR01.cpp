#include <Windows.h>
#include <tchar.h>
#include <string.h>
#include "resource.h"
#include <winres.h>

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
    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    WndClass.lpszClassName = _T("Window Class Name");

    RegisterClass(&WndClass);
    hwnd = CreateWindow(
        _T("Window Class Name"),
        _T("���Ѻ��� 6�� 1���� ������"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HANDLE hFile;
    HDC hdc;
    RECT rect;
    PAINTSTRUCT ps;
    OPENFILENAME OFN;
    static TCHAR filepath[1000], filename[100];
    TCHAR inbuff[1000];
    DWORD size;

    switch (iMsg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_NEWFILE:
            memset(&OFN, 0, sizeof(OPENFILENAME));
            OFN.lStructSize = sizeof(OPENFILENAME);
            OFN.hwndOwner = hwnd;
            OFN.lpstrFile = filepath;
            OFN.nMaxFile = 1000;
            OFN.lpstrFileTitle = filename;
            OFN.nMaxFileTitle = 100;
            OFN.lpstrFilter = _T("�ؽ�Ʈ ���� (*.txt)\0*.txt\0��� ���� (*.*)\0*.*\0");
            OFN.Flags = OFN_EXPLORER;
            OFN.lpstrInitialDir = _T("-");
            GetOpenFileName(&OFN);

            hFile = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                // Ŭ���̾�Ʈ ������ �����
                hdc = GetDC(hwnd);
                GetClientRect(hwnd, &rect);
                FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

                // ���ο� ������ ������ ���
                memset(inbuff, 0, sizeof(inbuff));
                ReadFile(hFile, inbuff, 999 * sizeof(TCHAR), &size, NULL);
                DrawText(hdc, inbuff, (int)_tcslen(inbuff), &rect, DT_TOP | DT_LEFT);

                ReleaseDC(hwnd, hdc);
                CloseHandle(hFile);
            }
            break;

            //ANSI���ڵ� ������ UNICODE ���Ϸ� �ٲ㼭 ����´�.
        case ID_AtoU:
            memset(&OFN, 0, sizeof(OPENFILENAME));
            OFN.lStructSize = sizeof(OPENFILENAME);
            OFN.hwndOwner = hwnd;
            OFN.lpstrFile = filepath;
            OFN.nMaxFile = 1000;
            OFN.lpstrFileTitle = filename;
            OFN.nMaxFileTitle = 100;
            OFN.lpstrFilter = _T("�ؽ�Ʈ ���� (*.txt)\0*.txt\0��� ���� (*.*)\0*.*\0");
            OFN.Flags = OFN_EXPLORER;
            OFN.lpstrInitialDir = _T("-");
            GetOpenFileName(&OFN);

            hFile = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                // Ŭ���̾�Ʈ ������ �����
                hdc = GetDC(hwnd);
                GetClientRect(hwnd, &rect);
                FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

                // ���ο� ������ ������ ���
                memset(inbuff, 0, sizeof(inbuff));
                ReadFile(hFile, inbuff, 999 * sizeof(TCHAR), &size, NULL);
                DrawText(hdc, inbuff, (int)_tcslen(inbuff), &rect, DT_TOP | DT_LEFT);

                ReleaseDC(hwnd, hdc);
                CloseHandle(hFile);
            }
            break;

            //UNICODE���ڵ� ������ ANSI ���Ϸ� �ٲ㼭 ����´�.
        case ID_UtoA:
            memset(&OFN, 0, sizeof(OPENFILENAME));
            OFN.lStructSize = sizeof(OPENFILENAME);
            OFN.hwndOwner = hwnd;
            OFN.lpstrFile = filepath;
            OFN.nMaxFile = 1000;
            OFN.lpstrFileTitle = filename;
            OFN.nMaxFileTitle = 100;
            OFN.lpstrFilter = _T("�ؽ�Ʈ ���� (*.txt)\0*.txt\0��� ���� (*.*)\0*.*\0");
            OFN.Flags = OFN_EXPLORER;
            OFN.lpstrInitialDir = _T("-");
            GetOpenFileName(&OFN);

            hFile = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                // Ŭ���̾�Ʈ ������ �����
                hdc = GetDC(hwnd);
                GetClientRect(hwnd, &rect);
                FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

                // ���ο� ������ ������ ���
                memset(inbuff, 0, sizeof(inbuff));
                ReadFile(hFile, inbuff, 999 * sizeof(TCHAR), &size, NULL);
                DrawText(hdc, inbuff, (int)_tcslen(inbuff), &rect, DT_TOP | DT_LEFT);

                ReleaseDC(hwnd, hdc);
                CloseHandle(hFile);
            }
            break;

        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
