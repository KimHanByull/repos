//�����쿡�� ����ϴ� API ����
//�پ��� �Լ��� ��ũ�� ����� ���� ��� ���� ���� ------------------- 1. ��� ����
#include <windows.h>



// - �ü���� ���� �޽����� �ü���� ���޵� �޽����� ������� ���� ���޵�
//WinMAin �Լ����� �����ϹǷ� �Լ� ���� ���� ------------------------ 2. ���� �Լ�
//HWND - ������ �ĺ���(�߻���Ų ������), UINT - �̺�Ʈ �޽��� �ĺ���, WPARAM - �̺�Ʈ �޽������� �ΰ� ����, LPARAM - �̺�Ʈ �߰� ����
//�̺�Ʈ�� ���� ������ ������ ���� �� �ֱ� ������ �Ű������� ���� �� ����
//lResult = define�� ���� ������ �� �� �ٸ� �ǹ� ����
//call �Լ��� ȣ���ϴ� �Լ� callback �Լ��� �ü���� ���� ȣ���� �Ǿ����� �Լ� 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		



//WINAPI - ������ ���α׷� �ǹ�
//hInstance - OS�� �ο��� ID�� �޸𸮿��� ��ġ, lpCmdLine - ��� ���ο��� ���α׷� ���� �� ���� ���ڿ�, nCmdShow - �����쿡 ����ϴ� ����


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//������ �⺻ ��� ������ Ŭ���� ����� �ü���� ���
	WNDCLASS wndclass;
	//�׿� ���� ������ â�� ����� �����ش�. h�� �ڵ��̶�� �����ϱ�, �����Ϳ� �����ϰ� �� ���� �뵵
	HWND hwnd;
	//�߻��Ǵ� �޽����� ���� �޽����� �ش��ϴ� ������ ��� ����ä
	MSG msg;

	//������ Ŭ������ �ʱ�ȭ�ϰ� �ü���� ����Ѵ�. 
	wndclass.style = CS_HREDRAW | CS_VREDRAW;						//��Ÿ�� ����
	wndclass.lpfnWndProc = WndProc;									//���ν��� �̸�, ������ �ڵ鷯 �ش��ϴ� �Լ� ���� 
	wndclass.cbClsExtra = 0;										//���� �޸�(0����Ʈ)
	wndclass.cbWndExtra = 0;										//���� �޸�(0����Ʈ)
	wndclass.hInstance = hInstance;									//�ν��Ͻ� �ڵ�
	wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION);					//������ ��� - ������ ? ���·� �����ϱ�
	wndclass.hCursor = LoadCursor(NULL, IDC_IBEAM);					//���콺 Ŀ�� ��� - ���콺 ������ �빮�� I ���·� �����ϱ�
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//��� �÷� - ���� ���������� �����ϱ�
	wndclass.lpszMenuName = NULL;									//�޴� â ����
	wndclass.lpszClassName = TEXT("HelloClass");
	if (!RegisterClass(&wndclass)) return 1;


	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("���Ѻ��� ù ��° ������"),		//Ÿ��Ʋ�ٿ� ���� �̸��� �̿��ϱ�
		WS_OVERLAPPEDWINDOW, 200, 300,												//�����찡 ��Ÿ�� ��ġ (200, 300)���� �����ϱ�
		600, 400, NULL, NULL, hInstance, NULL);										//������ â�� ũ�� (600 X 400)���� �����ϱ�
	ShowWindow(hwnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message) {
	case WM_CREATE:
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	
	return DefWindowProc(hwnd, message, wParam, lParam);

}