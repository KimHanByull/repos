
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

LRESULT CALLBACK FrameWndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {


	WNDCLASS wndclass;
	HWND hwnd;
	MSG msg;
	hInst = hInstance;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = FrameWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wndclass.hCursor = LoadCursor(NULL, IDC_IBEAM);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("Window Class Name");
	RegisterClass(&wndclass);
	hwnd = CreateWindow(
		TEXT("Window Class Name"),
		TEXT("���Ѻ��� 6W2D��° �θ� ���μ��� ������"),
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
	return (int)msg.wParam;
}

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndClient;
    CLIENTCREATESTRUCT clientcreate;
    MDICREATESTRUCT mdicreate;
    TCHAR str[30]; // �ڽ� �����찡 �����Կ� ���� ��ȣ ���̱� ����

    switch (iMsg)
    {
    case WM_CREATE:
        // �ڽ� ���μ��� ����
    {


        STARTUPINFO si = { sizeof(STARTUPINFO) };
        PROCESS_INFORMATION pi;
        DWORD dwExitCode;

        // ���⿡ �ڽ� ���μ��� ��θ� �ùٸ��� �������ּ���.

        BOOL fSuccess = CreateProcess(
            _T("C:\\Users\\IntekPlus\\source\\repos\\[W6D2]PR01\\Debug\\Child.exe"),
            NULL,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi);

        if (fSuccess)
        {
            CloseHandle(pi.hThread);
            // �ڽ� ���μ����� ����� ������ ���
            WaitForSingleObject(pi.hProcess, INFINITE);
            // �ڽ� ���μ����� ���� �ڵ� ��������
            GetExitCodeProcess(pi.hProcess, &dwExitCode);
            // �ڵ� ����
            CloseHandle(pi.hProcess);
            // �θ� ���μ��� ����
            PostQuitMessage(0);
        }
        else
        {
            // �������� ���� ó��
            MessageBox(hwnd, _T("�ڽ� ���μ����� ������ �� �����ϴ�."), _T("����"), MB_OK | MB_ICONERROR);
        }
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefFrameProc(hwnd, hwndClient, iMsg, wParam, lParam);
}
