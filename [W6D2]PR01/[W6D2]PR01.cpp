
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
		TEXT("김한별의 6W2D번째 부모 프로세스 윈도우"),
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
    TCHAR str[30]; // 자식 윈도우가 증가함에 따라 번호 붙이기 위함

    switch (iMsg)
    {
    case WM_CREATE:
        // 자식 프로세스 생성
    {


        STARTUPINFO si = { sizeof(STARTUPINFO) };
        PROCESS_INFORMATION pi;
        DWORD dwExitCode;

        // 여기에 자식 프로세스 경로를 올바르게 지정해주세요.

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
            // 자식 프로세스가 종료될 때까지 대기
            WaitForSingleObject(pi.hProcess, INFINITE);
            // 자식 프로세스의 종료 코드 가져오기
            GetExitCodeProcess(pi.hProcess, &dwExitCode);
            // 핸들 해제
            CloseHandle(pi.hProcess);
            // 부모 프로세스 종료
            PostQuitMessage(0);
        }
        else
        {
            // 실패했을 때의 처리
            MessageBox(hwnd, _T("자식 프로세스를 시작할 수 없습니다."), _T("에러"), MB_OK | MB_ICONERROR);
        }
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefFrameProc(hwnd, hwndClient, iMsg, wParam, lParam);
}
