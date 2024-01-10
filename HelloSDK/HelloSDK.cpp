//â �ϳ� ����� ���콺 �޽��� �ϳ� ó���ϴ� ���α׷�

//�����쿡�� ����ϴ� API ����
//�پ��� �Լ��� ��ũ�� ����� ���� ��� ���� ���� ------------------- 1. ��� ����
#include <windows.h>

//lResult = define�� ���� ������ �� �� �ٸ� �ǹ� ����
//call �Լ��� ȣ���ϴ� �Լ� callback �Լ��� �ü���� ���� ȣ���� �Ǿ����� �Լ� 
// - �ü���� ���� �޽����� �ü���� ���޵� �޽����� ������� ���� ���޵�
//WinMAin �Լ����� �����ϹǷ� �Լ� ���� ���� ------------------------ 2. ���� �Լ�
//HWND - ������ �ĺ���(�߻���Ų ������), UINT - �̺�Ʈ �޽��� �ĺ���, WPARAM - �̺�Ʈ �޽������� �ΰ� ����, LPARAM - �̺�Ʈ �߰� ����
//�̺�Ʈ�� ���� ������ ������ ���� �� �ֱ� ������ �Ű������� ���� �� ����
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//API�� ������ instance ������ 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR  lpCmdLine, int nCmdShow) {
	//������ �⺻ ��� ������ Ŭ���� ����� �ü���� ���
	WNDCLASS wndclass;
	//�׿� ���� ������ â�� ����� �����ش�. h�� �ڵ��̶�� �����ϱ�, �����Ϳ� �����ϰ� �� ���� �뵵
	HWND hwnd;
	//�߻��Ǵ� �޽����� ���� �޽����� �ش��ϴ� ������ ��� ����ä
	MSG msg;

	//������ Ŭ������ �ʱ�ȭ�ϰ� �ü���� ��� --------------------- 3. ������ Ŭ���� �ʱ�ȭ�� ���

	wndclass.style = CS_HREDRAW | CS_VREDRAW;						//��Ÿ�� ����
	//------------------------------------1�� ���� ����----------------------------
	//wndclass.style = CS_NOCLOSE;									//Ÿ��Ʋ�� �����ư ��Ȱ��ȭ

	wndclass.lpfnWndProc = WndProc;									//���ν��� �̸�, ������ �ڵ鷯 �ش��ϴ� �Լ� ���� 
	wndclass.cbClsExtra = 0;										//���� �޸�(0����Ʈ)
	wndclass.cbWndExtra = 0;										//���� �޸�(0����Ʈ)
	wndclass.hInstance = hInstance;									//�ν��Ͻ� �ڵ�

	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//������ ��� - â�� �ߴ� ������ ���
	//------------------------------------2�� ���� ����----------------------------
	//wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION);				//������ ��� - â�� �ߴ� ������ ���(?�� ����)

	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);					//Ŀ�� ���
	//------------------------------------3�� ���� ����----------------------------
	//wndclass.hCursor = LoadCursor(NULL, IDC_UPARROW);				//Ŀ�� ��� �� ȭ��ǥ
	
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//��� �÷� ���
	//------------------------------------4�� ���� ����----------------------------
	//wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//��� �÷� ������

	wndclass.lpszMenuName = NULL;									//�޴� â ����
	wndclass.lpszClassName = TEXT("HelloClass");					//������ Ŭ���� �̸�
	if (!RegisterClass(&wndclass)) return 1;			//���� ����

	//������ �����ϰ� ȭ�� ��� --------------------------------------- 4.������ ����
	//�Ű������� Ŭ���� �̸�, ������ �̸�, �����콺Ÿ��, x/y ��ǥ, �������� ���� ����, �θ� ������ �ڵ�, �޴� �ڵ�, �ν��Ͻ� �ڵ�, �ɼ� ������ ��
	//------------------------------------5�� ���� ����----------------------------
	// Ÿ��Ʋ�� �� ���� Hello SDK���� Hello World�� ����
	//hwnd = CreateWindow(TEXT("HelloClass"), TEXT("Hello World")
	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("HelloSDK"),	//�� ��° ���� �ٲ� �� Ÿ��Ʋ�� ���� ���� ����
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,		//�� ��° ���� ��Ÿ��, 4-5�� ���� ��ġ, 6-7�� ���� ũ�� ����
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	//------------------------------------6�� ���� ����----------------------------
	//�������� ������ ��� Ÿ��Ʋ��(ĸ��)�� �ִ� �˾� ������� ���� ������ ��ġ�� 0,0 ũ��� 500,200
	//hwnd = CreateWindow(TEXT("HelloClass"), TEXT("HelloSDK"),	//�� ��° ���� �ٲ� �� Ÿ��Ʋ�� ���� ���� ����
	//	WS_CAPTION|WS_POPUPWINDOW, 0, 0,		//�� ��° ���� ��Ÿ��, 4-5�� ���� ��ġ, 6-7�� ���� ũ�� ����
	//	500, 200, NULL, NULL, hInstance, NULL);


	//���������� â�� ������ �� ��
	//ù �Ű����� ������ �ڵ� �߿�, 
	//�� ��° ���ڰ� ������ ũ�� ���� - (nCmdShow - cmdâ ũ��, SW_MAXIMIZE - �ִ�ȭ
	//ShowWindow(hwnd, nCmdShow);
	//------------------------------------7�� ���� ����----------------------------
	//������ �ִ�ȭ ���·� ����
	ShowWindow(hwnd, SW_MAXIMIZE);
	//UpdateWindow(hwnd); ���� ����

	//�޽��� ť���� �޽����� �ϳ��� ������ ó���Ѵ�. ------------------ 5. �޽��� ����
	//get�޽������� ���� �� �޽��� ���� 0���� Ŀ�� ��
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		//Translate - �޽��� ���� �� ����
		TranslateMessage(&msg);
		//�޽��� ����
		//�޽��� ���� -> �� ó���� ���� WndProc���� ���� (callback �Լ�)
		DispatchMessage(&msg);
	}


	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	const TCHAR* str = TEXT("Hello, SDK");		//���� ����

	//�߻��� �޽����� ������ ���� ������ ó���Ѵ�.
	switch (message) {
		//������ �޽��� - WM 
		//â�� create ������� ��
	case WM_CREATE:
		return 0;
		//���콺 ���� ��ư ������ ��
		//���콺 ���� ��ư �ö�� �� - WMLBUTTONUP 
	case WM_LBUTTONDOWN:
		//------------------------------------8�� ���� ����----------------------------
		//MB_YESNO - ��/�ƴϿ� ���� ���
		//------------------------------------9�� ���� ����----------------------------
		//MB_ICONERROR
		//------------------------------------10�� ���� ����----------------------------
		//2�� ���� TEXT("�����Ͻðڽ��ϱ�?"), 4�� ����
		//MessageBox(hwnd, TEXT("���콺 Ŭ��!"), TEXT("���콺 �޽���"), MB_ICONERROR);
		//------------------------------------11�� ���� ����----------------------------
		//2�� ���� TEXT("�����Ͻðڽ��ϱ�?"), 4�� ����

		if(IDYES== MessageBox(hwnd, TEXT("�����Ͻðڽ��ϱ�?"), TEXT("���콺 �޽���"), MB_ICONQUESTION | MB_YESNO)){
			PostQuitMessage(0);
			return 0;
		}
	


		//ȭ���� ���̱� ���� �׸��� �۾�
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//���ڿ��� 100, 100�� ����϶�
		TextOut(hdc, 100, 100, str, lstrlen(str));
		EndPaint(hwnd, &ps);
		return 0;
		//����â ������ �۵�
	case WM_DESTROY:
		//�޽���ť�� postquititeem ��� 
		PostQuitMessage(0);
		return 0;
	}

	//���� ���α׷��� ó������ ���� �޽����� �ü���� ó��
	return DefWindowProc(hwnd, message, wParam, lParam);
}