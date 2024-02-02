
#include <windows.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASS wndclass;
	HWND hwnd;
	MSG msg;


	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wndclass.hCursor = LoadCursor(NULL, IDC_IBEAM);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("HelloClass");
	if (!RegisterClass(&wndclass)) return 1;


	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("���Ѻ��� �� ��° ������"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
		CW_USEDEFAULT, CW_USEDEFAULT
		, NULL, NULL, hInstance, NULL);
    //�簢���� ũ�� ����Ͽ� ȭ���� FULL�� ����
	ShowWindow(hwnd, SW_MAXIMIZE);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    //�簢�� ��ǥ    
    static RECT rectTop = { 300, 100, 500, 300 };
    static RECT rectLeft = { 100, 300, 300, 500 };
    static RECT rectRight = { 500, 300, 700, 500 };
    static RECT rectBottom = { 300, 500, 500, 700 };

    //VK�� �� ���¸� ������ ����(������ ��� TRUE)
    static bool isUpKeyPressed = false;
    static bool isLeftKeyPressed = false;
    static bool isRightKeyPressed = false;
    static bool isDownKeyPressed = false;

    HDC hdc;
    PAINTSTRUCT ps;
    HBRUSH hbrush, oldbrush;

    switch (message) {
    case WM_CREATE:
        return 0;

    case WM_PAINT:

        //�ʱ� ȭ�� ���ڸ� �׸��� �߽ɿ� ���� ��ġ�� �˷���
        hdc = BeginPaint(hwnd, &ps);
        Rectangle(hdc, rectTop.left, rectTop.top, rectTop.right, rectTop.bottom);
        DrawText(hdc, TEXT("����"), 2, &rectTop, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        Rectangle(hdc, rectLeft.left, rectLeft.top, rectLeft.right, rectLeft.bottom);
        DrawText(hdc, TEXT("����"), 2, &rectLeft, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        Rectangle(hdc, rectRight.left, rectRight.top, rectRight.right, rectRight.bottom);
        DrawText(hdc, TEXT("������"), 3, &rectRight, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        Rectangle(hdc, rectBottom.left, rectBottom.top, rectBottom.right, rectBottom.bottom);
        DrawText(hdc, TEXT("�Ʒ���"), 3, &rectBottom, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


        // VK_UP Ű�� ������ �� ���� ������ ���� ���� TRUE�� ���� ���������� �簢���� �׸�
        if (isUpKeyPressed) {
            hbrush = CreateSolidBrush(RGB(255, 0, 0));
            oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
            Rectangle(hdc, rectTop.left, rectTop.top, rectTop.right, rectTop.bottom);
            SelectObject(hdc, oldbrush);
            DeleteObject(hbrush);
        }

        // VK_LEFT Ű�� ������ �� ���� ������ ���� ���� TRUE�� ���� ���������� �簢���� �׸�
        if (isLeftKeyPressed) {
            hbrush = CreateSolidBrush(RGB(255, 0, 0));
            oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
            Rectangle(hdc, rectLeft.left, rectLeft.top, rectLeft.right, rectLeft.bottom);
            SelectObject(hdc, oldbrush);
            DeleteObject(hbrush);
        }

        // VK_RIGHT Ű�� ������ �� ���� ������ ���� ���� TRUE�� ���� ���������� �簢���� �׸�
        if (isRightKeyPressed) {
            hbrush = CreateSolidBrush(RGB(255, 0, 0));
            oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
            Rectangle(hdc, rectRight.left, rectRight.top, rectRight.right, rectRight.bottom);
            SelectObject(hdc, oldbrush);
            DeleteObject(hbrush);
        }

        // VK_DOWN Ű�� ������ �� ���� ������ ���� ���� TRUE�� ���� ���������� �簢���� �׸�
        if (isDownKeyPressed) {
            hbrush = CreateSolidBrush(RGB(255, 0, 0));
            oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
            Rectangle(hdc, rectBottom.left, rectBottom.top, rectBottom.right, rectBottom.bottom);
            SelectObject(hdc, oldbrush);
            DeleteObject(hbrush);
        }

        EndPaint(hwnd, &ps);
        return 0;


    //����Ű�� ������ �� ������ ���� TRUE������ ����
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_UP:
            isUpKeyPressed = true;
            InvalidateRect(hwnd, &rectTop, TRUE);
            return 0;

        case VK_LEFT:
            isLeftKeyPressed = true;
            InvalidateRect(hwnd, &rectLeft, TRUE);
            return 0;

        case VK_RIGHT:
            isRightKeyPressed = true;
            InvalidateRect(hwnd, &rectRight, TRUE);
            return 0;

        case VK_DOWN:
            isDownKeyPressed = true;
            InvalidateRect(hwnd, &rectBottom, TRUE);
            return 0;
        }

    //����Ű ������ Ǯ���� ��� FALSE�� �ٽ� �����Ͽ� ���� �簢�� ����� ����
    case WM_KEYUP:
        switch (wParam) {
        case VK_UP:
            isUpKeyPressed = false;
            InvalidateRect(hwnd, &rectTop, TRUE);
            return 0;

        case VK_LEFT:
            isLeftKeyPressed = false;
            InvalidateRect(hwnd, &rectLeft, TRUE);
            return 0;

        case VK_RIGHT:
            isRightKeyPressed = false;
            InvalidateRect(hwnd, &rectRight, TRUE);
            return 0;

        case VK_DOWN:
            isDownKeyPressed = false;
            InvalidateRect(hwnd, &rectBottom, TRUE);
            return 0;


        }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);

}