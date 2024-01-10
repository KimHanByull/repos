//창 하나 만들고 마우스 메시지 하나 처리하는 프로그램

//윈도우에서 사용하는 API 파일
//다양한 함수와 메크로 사용을 위한 헤더 파일 참조 ------------------- 1. 헤더 파일
#include <windows.h>

//lResult = define에 의해 재정의 된 것 다른 의미 없음
//call 함수는 호출하는 함수 callback 함수는 운영체제에 의해 호출이 되어지는 함수 
// - 운영체제에 들어온 메시지가 운영체제에 전달되 메시지가 윈도우로 들어와 전달됨
//WinMAin 함수에서 참조하므로 함수 원형 선언 ------------------------ 2. 메인 함수
//HWND - 윈도우 식별자(발생시킨 윈도우), UINT - 이벤트 메시지 식별자, WPARAM - 이벤트 메시지에서 부가 정보, LPARAM - 이벤트 추가 정보
//이벤트가 여러 정보를 가지고 있을 수 있기 때문에 매개변수도 여러 개 만듬
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//API에 진입점 instance 정보들 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR  lpCmdLine, int nCmdShow) {
	//윈도우 기본 골격 윈도우 클래스 만들고 운영체제에 등록
	WNDCLASS wndclass;
	//그에 따른 윈도우 창을 만들고 보여준다. h는 핸들이라고 생각하기, 데이터에 접근하고 값 변경 용도
	HWND hwnd;
	//발생되는 메시지를 저장 메시지에 해당하는 정보를 담는 구조채
	MSG msg;

	//윈도우 클래스를 초기화하고 운영체제에 등록 --------------------- 3. 윈도우 클래스 초기화와 등록

	wndclass.style = CS_HREDRAW | CS_VREDRAW;						//스타일 지정
	//------------------------------------1번 연습 문제----------------------------
	//wndclass.style = CS_NOCLOSE;									//타이틀바 종료버튼 비활성화

	wndclass.lpfnWndProc = WndProc;									//프로시저 이름, 윈도우 핸들러 해당하는 함수 지정 
	wndclass.cbClsExtra = 0;										//여분 메모리(0바이트)
	wndclass.cbWndExtra = 0;										//여분 메모리(0바이트)
	wndclass.hInstance = hInstance;									//인스턴스 핸들

	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//아이콘 모양 - 창에 뜨는 아이콘 모양
	//------------------------------------2번 연습 문제----------------------------
	//wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION);				//아이콘 모양 - 창에 뜨는 아이콘 모양(?로 변경)

	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);					//커서 모양
	//------------------------------------3번 연습 문제----------------------------
	//wndclass.hCursor = LoadCursor(NULL, IDC_UPARROW);				//커서 모양 위 화살표
	
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//배경 컬러 흰색
	//------------------------------------4번 연습 문제----------------------------
	//wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//배경 컬러 검정색

	wndclass.lpszMenuName = NULL;									//메뉴 창 없음
	wndclass.lpszClassName = TEXT("HelloClass");					//윈도우 클래스 이름
	if (!RegisterClass(&wndclass)) return 1;			//여기 차이

	//윈도우 생성하고 화면 출력 --------------------------------------- 4.윈도우 생성
	//매개변수는 클래스 이름, 윈도우 이름, 윈도우스타일, x/y 좌표, 윈도우의 폭과 높이, 부모 윈도우 핸들, 메뉴 핸들, 인스턴스 핸들, 옵션 데이터 순
	//------------------------------------5번 연습 문제----------------------------
	// 타이틀바 내 글자 Hello SDK에서 Hello World로 수정
	//hwnd = CreateWindow(TEXT("HelloClass"), TEXT("Hello World")
	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("HelloSDK"),	//두 번째 인자 바꿀 시 타이틀바 제목 변경 가능
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,		//세 번째 인자 스타일, 4-5번 인자 위치, 6-7번 인자 크기 지정
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	//------------------------------------6번 연습 문제----------------------------
	//오버랩된 윈도우 대신 타이틀바(캡션)가 있는 팝업 윈도우로 수정 윈도우 위치는 0,0 크기는 500,200
	//hwnd = CreateWindow(TEXT("HelloClass"), TEXT("HelloSDK"),	//두 번째 인자 바꿀 시 타이틀바 제목 변경 가능
	//	WS_CAPTION|WS_POPUPWINDOW, 0, 0,		//세 번째 인자 스타일, 4-5번 인자 위치, 6-7번 인자 크기 지정
	//	500, 200, NULL, NULL, hInstance, NULL);


	//직접적으로 창을 보고자 할 때
	//첫 매개변수 윈도우 핸들 중요, 
	//두 번째 인자가 윈도우 크기 관련 - (nCmdShow - cmd창 크기, SW_MAXIMIZE - 최대화
	//ShowWindow(hwnd, nCmdShow);
	//------------------------------------7번 연습 문제----------------------------
	//윈도우 최대화 상태로 실행
	ShowWindow(hwnd, SW_MAXIMIZE);
	//UpdateWindow(hwnd); 여기 차이

	//메시지 큐에서 메시지를 하나씩 꺼내서 처리한다. ------------------ 5. 메시지 루프
	//get메시지에서 꺼내 온 메시지 값이 0보다 커질 때
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		//Translate - 메시지 가공 후 정리
		TranslateMessage(&msg);
		//메시지 전달
		//메시지 배포 -> 맨 처음에 나온 WndProc으로 전달 (callback 함수)
		DispatchMessage(&msg);
	}


	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	const TCHAR* str = TEXT("Hello, SDK");		//여기 차이

	//발생한 메시지의 종류에 따라 적절히 처리한다.
	switch (message) {
		//윈도우 메시지 - WM 
		//창이 create 만들어질 때
	case WM_CREATE:
		return 0;
		//마우스 왼쪽 버튼 눌렀을 때
		//마우스 왼쪽 버튼 올라올 때 - WMLBUTTONUP 
	case WM_LBUTTONDOWN:
		//------------------------------------8번 연습 문제----------------------------
		//MB_YESNO - 예/아니요 동시 출력
		//------------------------------------9번 연습 문제----------------------------
		//MB_ICONERROR
		//------------------------------------10번 연습 문제----------------------------
		//2번 인자 TEXT("종료하시겠습니까?"), 4번 인자
		//MessageBox(hwnd, TEXT("마우스 클릭!"), TEXT("마우스 메시지"), MB_ICONERROR);
		//------------------------------------11번 연습 문제----------------------------
		//2번 인자 TEXT("종료하시겠습니까?"), 4번 인자

		if(IDYES== MessageBox(hwnd, TEXT("종료하시겠습니까?"), TEXT("마우스 메시지"), MB_ICONQUESTION | MB_YESNO)){
			PostQuitMessage(0);
			return 0;
		}
	


		//화면을 보이기 전에 그리는 작업
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//문자열을 100, 100에 출력하라
		TextOut(hdc, 100, 100, str, lstrlen(str));
		EndPaint(hwnd, &ps);
		return 0;
		//종료창 누를때 작동
	case WM_DESTROY:
		//메시지큐에 postquititeem 등록 
		PostQuitMessage(0);
		return 0;
	}

	//응용 프로그램이 처리하지 않은 메시지는 운영체제가 처리
	return DefWindowProc(hwnd, message, wParam, lParam);
}