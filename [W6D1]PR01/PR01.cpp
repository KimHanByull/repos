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

                    // ���� ���� �б�
                    char* textBuffer = new char[fileSize + 1];
                    memset(textBuffer, 0, fileSize + 1);

                    if (ReadFile(hFile, textBuffer, fileSize, &size, NULL)) {
                        // ������ ���ڵ� ��� Ȯ��
                        if (IsTextUnicode(textBuffer, fileSize, 0)) {
                            // Unicode�� ��� �ƹ��͵� ���� �ʰ� ���� ���� ���
                            DrawTextW(hdc, (WCHAR*)textBuffer, (int)(fileSize / sizeof(WCHAR)), &rect, DT_TOP | DT_LEFT);
                        }
                        else {
                            // ANSI�� ��� �����ڵ�� ��ȯ
                            int unicodeBufferSize = MultiByteToWideChar(CP_ACP, 0, textBuffer, -1, NULL, 0);
                            WCHAR* unicodeBuffer = new WCHAR[unicodeBufferSize];
                            MultiByteToWideChar(CP_ACP, 0, textBuffer, -1, unicodeBuffer, unicodeBufferSize);

                            // �����ڵ� ���ڿ� ���
                            DrawTextW(hdc, unicodeBuffer, (int)wcslen(unicodeBuffer), &rect, DT_TOP | DT_LEFT);

                            //�����ڵ� ������ �ٸ��̸����� �����ϱ� ���� �ڵ�
                            memset(&SFN, 0, sizeof(OPENFILENAME));
                            SFN.lStructSize = sizeof(OPENFILENAME);
                            SFN.hwndOwner = hwnd;
                            SFN.nMaxFile = 256;
                            SFN.lpstrFilter = _T("�ؽ�Ʈ ���� (*.txt)\0*.txt\0��� ���� (*.*)\0*.*\0");
                            SFN.lpstrFile = filepath;
                            SFN.lpstrInitialDir = _T(".");

                            //�� ���� �����ϱ�
                            if (GetSaveFileName(&SFN) != 0) {
                                FileSave(hwnd, SFN.lpstrFile, unicodeBuffer);
                            }

                            // ���� �Ҵ�� �޸� ����
                            delete[] unicodeBuffer;
                        }
                    }
                    else {
                        // ���� �б⿡ �������� ��� ���� ó��
                        MessageBox(hwnd, _T("���� �б⿡ �����߽��ϴ�."), _T("����"), MB_OK | MB_ICONERROR);
                    }

                    // ���� �Ҵ�� �޸� ����
                    delete[] textBuffer;

                    ReleaseDC(hwnd, hdc);
                    CloseHandle(hFile);
                }
                else {
                    // ���� ���⿡ �������� ��� ���� ó��
                    MessageBox(hwnd, _T("���� ���⿡ �����߽��ϴ�."), _T("����"), MB_OK | MB_ICONERROR);
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
    HANDLE hFile;           //�����ϱ� ���� ���� �ڵ�
    int size;               //�ִ� ����Ʈ �� ���� �뵵
    WORD uni = 0xFEFF;      //�����ڵ����� ǥ��
    DWORD nSize;            //���Ͽ� ������ ����Ʈ ��

    hFile = CreateFile(
        fileSaveName,       //���� ���� �̸�
        GENERIC_WRITE,      //�����
        0,                  //���� ����
        0, 
        CREATE_ALWAYS,      //�׻� ����� (���� ��� ���� ����, ���� ��� ���� �����)
        FILE_ATTRIBUTE_NORMAL, 0);
    WriteFile(hFile, &uni, 2, &nSize, NULL);

    size = wcslen(content);
    WriteFile(hFile, content, size * sizeof(WCHAR), (LPDWORD)&size, NULL);

    CloseHandle(hFile);
}