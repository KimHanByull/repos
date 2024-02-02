//윈도우에서 사용하는 API 파일
//다양한 함수와 메크로 사용을 위한 헤더 파일 참조 ------------------- 1. 헤더 파일
#include <windows.h>



// - 운영체제에 들어온 메시지가 운영체제에 전달되 메시지가 윈도우로 들어와 전달됨
//WinMAin 함수에서 참조하므로 함수 원형 선언 ------------------------ 2. 메인 함수
//HWND - 윈도우 식별자(발생시킨 윈도우), UINT - 이벤트 메시지 식별자, WPARAM - 이벤트 메시지에서 부가 정보, LPARAM - 이벤트 추가 정보
//이벤트가 여러 정보를 가지고 있을 수 있기 때문에 매개변수도 여러 개 만듬
//lResult = define에 의해 재정의 된 것 다른 의미 없음
//call 함수는 호출하는 함수 callback 함수는 운영체제에 의해 호출이 되어지는 함수 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		



//WINAPI - 윈도우 프로그램 의미
//hInstance - OS가 부여한 ID로 메모리에서 위치, lpCmdLine - 명령 라인에서 프로그램 구동 시 전달 문자열, nCmdShow - 윈도우에 출력하는 형태


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//윈도우 기본 골격 윈도우 클래스 만들고 운영체제에 등록
	WNDCLASS wndclass;
	//그에 따른 윈도우 창을 만들고 보여준다. h는 핸들이라고 생각하기, 데이터에 접근하고 값 변경 용도
	HWND hwnd;
	//발생되는 메시지를 저장 메시지에 해당하는 정보를 담는 구조채
	MSG msg;

	//윈도우 클래스를 초기화하고 운영체제에 등록한다. 
	wndclass.style = CS_HREDRAW | CS_VREDRAW;						//스타일 지정
	wndclass.lpfnWndProc = WndProc;									//프로시저 이름, 윈도우 핸들러 해당하는 함수 지정 
	wndclass.cbClsExtra = 0;										//여분 메모리(0바이트)
	wndclass.cbWndExtra = 0;										//여분 메모리(0바이트)
	wndclass.hInstance = hInstance;									//인스턴스 핸들
	wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION);					//아이콘 모양 - 아이콘 ? 형태로 변경하기
	wndclass.hCursor = LoadCursor(NULL, IDC_IBEAM);					//마우스 커서 모양 - 마우스 아이콘 대문자 I 형태로 변경하기
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//배경 컬러 - 배경색 검은색으로 변경하기
	wndclass.lpszMenuName = NULL;									//메뉴 창 없음
	wndclass.lpszClassName = TEXT("HelloClass");
	if (!RegisterClass(&wndclass)) return 1;


	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("김한별의 첫 번째 윈도우"),		//타이틀바에 본인 이름을 이용하기
		WS_OVERLAPPEDWINDOW, 200, 300,												//윈도우가 나타날 위치 (200, 300)으로 변경하기
		600, 400, NULL, NULL, hInstance, NULL);										//윈도우 창의 크기 (600 X 400)으로 변경하기
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