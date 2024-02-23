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
        _T("김한별의 6주 1일차 윈도우"),
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
            OFN.lpstrFilter = _T("텍스트 파일 (*.txt)\0*.txt\0모든 파일 (*.*)\0*.*\0");
            OFN.Flags = OFN_EXPLORER;
            OFN.lpstrInitialDir = _T("-");
            GetOpenFileName(&OFN);

            hFile = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                // 클라이언트 영역을 지우기
                hdc = GetDC(hwnd);
                GetClientRect(hwnd, &rect);
                FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

                // 새로운 파일의 내용을 출력
                memset(inbuff, 0, sizeof(inbuff));
                ReadFile(hFile, inbuff, 999 * sizeof(TCHAR), &size, NULL);
                DrawText(hdc, inbuff, (int)_tcslen(inbuff), &rect, DT_TOP | DT_LEFT);

                ReleaseDC(hwnd, hdc);
                CloseHandle(hFile);
            }
            break;

            //ANSI인코딩 파일을 UNICODE 파일로 바꿔서 열어온다.
        case ID_AtoU:
            memset(&OFN, 0, sizeof(OPENFILENAME));
            OFN.lStructSize = sizeof(OPENFILENAME);
            OFN.hwndOwner = hwnd;
            OFN.lpstrFile = filepath;
            OFN.nMaxFile = 1000;
            OFN.lpstrFileTitle = filename;
            OFN.nMaxFileTitle = 100;
            OFN.lpstrFilter = _T("텍스트 파일 (*.txt)\0*.txt\0모든 파일 (*.*)\0*.*\0");
            OFN.Flags = OFN_EXPLORER;
            OFN.lpstrInitialDir = _T("-");
            GetOpenFileName(&OFN);

            hFile = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                // 클라이언트 영역을 지우기
                hdc = GetDC(hwnd);
                GetClientRect(hwnd, &rect);
                FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

                // 파일 크기 확인
                DWORD fileSize = GetFileSize(hFile, NULL);
                if (fileSize == INVALID_FILE_SIZE) {
                    // 파일 크기 확인에 실패했을 경우 에러 처리
                    MessageBox(hwnd, _T("파일 크기 확인에 실패했습니다."), _T("에러"), MB_OK | MB_ICONERROR);
                    return 0;
                }

                // 파일 크기만큼의 버퍼 할당
                char* ansiBuffer = new char[fileSize + 1];
                memset(ansiBuffer, 0, fileSize + 1);

                // 파일 내용 읽기
                if (ReadFile(hFile, ansiBuffer, fileSize, &size, NULL)) {
                    // ANSI를 유니코드로 변환
                    int unicodeBufferSize = MultiByteToWideChar(CP_ACP, 0, ansiBuffer, -1, NULL, 0);
                    WCHAR* unicodeBuffer = new WCHAR[unicodeBufferSize];
                    MultiByteToWideChar(CP_ACP, 0, ansiBuffer, -1, unicodeBuffer, unicodeBufferSize);

                    // 유니코드 문자열 출력
                    DrawText(hdc, unicodeBuffer, (int)wcslen(unicodeBuffer), &rect, DT_TOP | DT_LEFT);

                    // 동적 할당된 메모리 해제
                    delete[] unicodeBuffer;
                }
                else {
                    // 파일 읽기에 실패했을 경우 에러 처리
                    MessageBox(hwnd, _T("파일 읽기에 실패했습니다."), _T("에러"), MB_OK | MB_ICONERROR);
                }

                // 동적 할당된 메모리 해제
                delete[] ansiBuffer;

                ReleaseDC(hwnd, hdc);
                CloseHandle(hFile);
            }
            else {
                // 파일 열기에 실패했을 경우 에러 처리
                MessageBox(hwnd, _T("파일 열기에 실패했습니다."), _T("에러"), MB_OK | MB_ICONERROR);
            }
            break;

            //UNICODE인코딩 파일을 ANSI 파일로 바꿔서 열어온다.
        case ID_UtoA:
            memset(&OFN, 0, sizeof(OPENFILENAME));
            OFN.lStructSize = sizeof(OPENFILENAME);
            OFN.hwndOwner = hwnd;
            OFN.lpstrFile = filepath;
            OFN.nMaxFile = 1000;
            OFN.lpstrFileTitle = filename;
            OFN.nMaxFileTitle = 100;
            OFN.lpstrFilter = _T("텍스트 파일 (*.txt)\0*.txt\0모든 파일 (*.*)\0*.*\0");
            OFN.Flags = OFN_EXPLORER;
            OFN.lpstrInitialDir = _T("-");
            GetOpenFileName(&OFN);

            hFile = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                // 클라이언트 영역을 지우기
                hdc = GetDC(hwnd);
                GetClientRect(hwnd, &rect);
                FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

                // 파일 크기 확인
                DWORD fileSize = GetFileSize(hFile, NULL);
                if (fileSize == INVALID_FILE_SIZE) {
                    // 파일 크기 확인에 실패했을 경우 에러 처리
                    MessageBox(hwnd, _T("파일 크기 확인에 실패했습니다."), _T("에러"), MB_OK | MB_ICONERROR);
                    return 0;
                }

                // 파일 크기만큼의 버퍼 할당
                WCHAR* unicodeBuffer = new WCHAR[fileSize + 1];
                memset(unicodeBuffer, 0, (fileSize + 1)*sizeof(WCHAR));

                // 파일 내용 읽기
                if (ReadFile(hFile, unicodeBuffer, fileSize, &size, NULL)) {
                    // UNICODE를 ANSI로 변환
                    int AnsiBufferSize = WideCharToMultiByte(CP_ACP, 0, unicodeBuffer, -1, NULL, 0, NULL, NULL);
                    char* ansiBuffer = new char[AnsiBufferSize];
                    WideCharToMultiByte(CP_ACP, 0, unicodeBuffer, -1, ansiBuffer, AnsiBufferSize, NULL, NULL);

                    // ANSI 문자열 출력
                    DrawTextA(hdc, ansiBuffer, (int)strlen(ansiBuffer), &rect, DT_TOP | DT_LEFT);

                    // 동적 할당된 메모리 해제
                    delete[] ansiBuffer;
                }
                else {
                    // 파일 읽기에 실패했을 경우 에러 처리
                    MessageBox(hwnd, _T("파일 읽기에 실패했습니다."), _T("에러"), MB_OK | MB_ICONERROR);
                }

                // 동적 할당된 메모리 해제
                delete[] unicodeBuffer;

                ReleaseDC(hwnd, hdc);
                CloseHandle(hFile);
            }
            else {
                // 파일 열기에 실패했을 경우 에러 처리
                MessageBox(hwnd, _T("파일 열기에 실패했습니다."), _T("에러"), MB_OK | MB_ICONERROR);
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
