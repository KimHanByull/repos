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

                // ���� ũ�� Ȯ��
                DWORD fileSize = GetFileSize(hFile, NULL);
                if (fileSize == INVALID_FILE_SIZE) {
                    // ���� ũ�� Ȯ�ο� �������� ��� ���� ó��
                    MessageBox(hwnd, _T("���� ũ�� Ȯ�ο� �����߽��ϴ�."), _T("����"), MB_OK | MB_ICONERROR);
                    return 0;
                }

                // ���� ũ�⸸ŭ�� ���� �Ҵ�
                char* ansiBuffer = new char[fileSize + 1];
                memset(ansiBuffer, 0, fileSize + 1);

                // ���� ���� �б�
                if (ReadFile(hFile, ansiBuffer, fileSize, &size, NULL)) {
                    // ANSI�� �����ڵ�� ��ȯ
                    int unicodeBufferSize = MultiByteToWideChar(CP_ACP, 0, ansiBuffer, -1, NULL, 0);
                    WCHAR* unicodeBuffer = new WCHAR[unicodeBufferSize];
                    MultiByteToWideChar(CP_ACP, 0, ansiBuffer, -1, unicodeBuffer, unicodeBufferSize);

                    // �����ڵ� ���ڿ� ���
                    DrawText(hdc, unicodeBuffer, (int)wcslen(unicodeBuffer), &rect, DT_TOP | DT_LEFT);

                    // ���� �Ҵ�� �޸� ����
                    delete[] unicodeBuffer;
                }
                else {
                    // ���� �б⿡ �������� ��� ���� ó��
                    MessageBox(hwnd, _T("���� �б⿡ �����߽��ϴ�."), _T("����"), MB_OK | MB_ICONERROR);
                }

                // ���� �Ҵ�� �޸� ����
                delete[] ansiBuffer;

                ReleaseDC(hwnd, hdc);
                CloseHandle(hFile);
            }
            else {
                // ���� ���⿡ �������� ��� ���� ó��
                MessageBox(hwnd, _T("���� ���⿡ �����߽��ϴ�."), _T("����"), MB_OK | MB_ICONERROR);
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

                // ���� ũ�� Ȯ��
                DWORD fileSize = GetFileSize(hFile, NULL);
                if (fileSize == INVALID_FILE_SIZE) {
                    // ���� ũ�� Ȯ�ο� �������� ��� ���� ó��
                    MessageBox(hwnd, _T("���� ũ�� Ȯ�ο� �����߽��ϴ�."), _T("����"), MB_OK | MB_ICONERROR);
                    return 0;
                }

                // ���� ũ�⸸ŭ�� ���� �Ҵ�
                WCHAR* unicodeBuffer = new WCHAR[fileSize + 1];
                memset(unicodeBuffer, 0, (fileSize + 1)*sizeof(WCHAR));

                // ���� ���� �б�
                if (ReadFile(hFile, unicodeBuffer, fileSize, &size, NULL)) {
                    // UNICODE�� ANSI�� ��ȯ
                    int AnsiBufferSize = WideCharToMultiByte(CP_ACP, 0, unicodeBuffer, -1, NULL, 0, NULL, NULL);
                    char* ansiBuffer = new char[AnsiBufferSize];
                    WideCharToMultiByte(CP_ACP, 0, unicodeBuffer, -1, ansiBuffer, AnsiBufferSize, NULL, NULL);

                    // ANSI ���ڿ� ���
                    DrawTextA(hdc, ansiBuffer, (int)strlen(ansiBuffer), &rect, DT_TOP | DT_LEFT);

                    // ���� �Ҵ�� �޸� ����
                    delete[] ansiBuffer;
                }
                else {
                    // ���� �б⿡ �������� ��� ���� ó��
                    MessageBox(hwnd, _T("���� �б⿡ �����߽��ϴ�."), _T("����"), MB_OK | MB_ICONERROR);
                }

                // ���� �Ҵ�� �޸� ����
                delete[] unicodeBuffer;

                ReleaseDC(hwnd, hdc);
                CloseHandle(hFile);
            }
            else {
                // ���� ���⿡ �������� ��� ���� ó��
                MessageBox(hwnd, _T("���� ���⿡ �����߽��ϴ�."), _T("����"), MB_OK | MB_ICONERROR);
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
