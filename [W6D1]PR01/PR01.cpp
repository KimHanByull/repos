#include <Windows.h>
#include <tchar.h>
#include <string.h>
#include "resource.h"
#include <winres.h>
#include <winbase.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;
void FileSave(HWND hwnd, LPTSTR fileSaveName, WCHAR* content);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	hInst = hInstance;
    HMODULE hmodule;

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
	OPENFILENAME OFN, SFN;
	static TCHAR filepath[1000], filename[100];
	TCHAR inbuff[1000];
	DWORD size;

	switch (iMsg)
	{
	case WM_COMMAND:
        switch (LOWORD(wParam))
        {
            case ID_NEWFILE:
            {
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

                    // 파일 내용 읽기
                    char* textBuffer = new char[fileSize + 1];
                    memset(textBuffer, 0, fileSize + 1);

                    if (ReadFile(hFile, textBuffer, fileSize, &size, NULL)) {
                        // 파일의 인코딩 방식 확인
                        if (IsTextUnicode(textBuffer, fileSize, 0)) {
                            // Unicode인 경우 아무것도 하지 않고 기존 파일 출력
                            DrawTextW(hdc, (WCHAR*)textBuffer, (int)(fileSize / sizeof(WCHAR)), &rect, DT_TOP | DT_LEFT);
                        }
                        else {
                            // ANSI일 경우 유니코드로 변환
                            int unicodeBufferSize = MultiByteToWideChar(CP_ACP, 0, textBuffer, -1, NULL, 0);
                            WCHAR* unicodeBuffer = new WCHAR[unicodeBufferSize];
                            MultiByteToWideChar(CP_ACP, 0, textBuffer, -1, unicodeBuffer, unicodeBufferSize);

                            // 유니코드 문자열 출력
                            DrawTextW(hdc, unicodeBuffer, (int)wcslen(unicodeBuffer), &rect, DT_TOP | DT_LEFT);

                            //유니코드 파일을 다른이름으로 저장하기 위한 코드
                            memset(&SFN, 0, sizeof(OPENFILENAME));
                            SFN.lStructSize = sizeof(OPENFILENAME);
                            SFN.hwndOwner = hwnd;
                            SFN.nMaxFile = 256;
                            SFN.lpstrFilter = _T("텍스트 파일 (*.txt)\0*.txt\0모든 파일 (*.*)\0*.*\0");
                            SFN.lpstrFile = filepath;
                            SFN.lpstrInitialDir = _T(".");

                            //연 파일 저장하기
                            if (GetSaveFileName(&SFN) != 0) {
                                FileSave(hwnd, SFN.lpstrFile, unicodeBuffer);
                            }

                            // 동적 할당된 메모리 해제
                            delete[] unicodeBuffer;
                        }
                    }
                    else {
                        // 파일 읽기에 실패했을 경우 에러 처리
                        MessageBox(hwnd, _T("파일 읽기에 실패했습니다."), _T("에러"), MB_OK | MB_ICONERROR);
                    }

                    // 동적 할당된 메모리 해제
                    delete[] textBuffer;

                    ReleaseDC(hwnd, hdc);
                    CloseHandle(hFile);
                }
                else {
                    // 파일 열기에 실패했을 경우 에러 처리
                    MessageBox(hwnd, _T("파일 열기에 실패했습니다."), _T("에러"), MB_OK | MB_ICONERROR);
                }
            }
        }
        break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void FileSave(HWND hwnd, LPTSTR fileSaveName, WCHAR * content) {
    HANDLE hFile;           //저장하기 위한 파일 핸들
    int size;               //최대 바이트 수 구할 용도
    WORD uni = 0xFEFF;      //유니코드임을 표현
    DWORD nSize;            //파일에 쓰여진 바이트 수

    hFile = CreateFile(
        fileSaveName,       //저장 위한 이름
        GENERIC_WRITE,      //쓰기용
        0,                  //공유 금지
        0, 
        CREATE_ALWAYS,      //항상 만들기 (있을 경우 덮어 쓰고, 없을 경우 새로 만들기)
        FILE_ATTRIBUTE_NORMAL, 0);
    WriteFile(hFile, &uni, 2, &nSize, NULL);

    size = wcslen(content);
    WriteFile(hFile, content, size * sizeof(WCHAR), (LPDWORD)&size, NULL);

    CloseHandle(hFile);
}