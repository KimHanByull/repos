//헤더 파일 선언
#include <afxwin.h>

//응용 프로그램 클래스 선언
//응용 프로그램의 초기화, 메시지 루프, 종료 코드 제공
class CHelloApp : public CWinApp 
{
public:
	virtual BOOL InitInstance();
};

//메인 윈도우 클래스 선언
//메인 윈도우 기능 제공
class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

//응용 프로그램 객체를 선언
//CWinApp 클래스를 상속받아 클래스를 만들고 이 클래스로 객체 하나를 전역 변수(응용 프로그램 객체)로 만듬(하나만 존재 가능)
CHelloApp theApp;

//응용 프로그램 클래스를 정의
//인스턴스 초기화 - 응용프로그램 초기화 -> 프레임 하나 생성 -> 화면에 띄우는 역할
//main 함수 없이 진행되어 헷갈림 (내부에 숨겨져 있는 부분)
//InitInstance 함수 호출과 재정의 필수
//상속받아 사용하는 가상함수로 매우 중요
BOOL CHelloApp::InitInstance()
{
	m_pMainWnd = new CMainFrame;
	//SDK ShowWindow와 같은 역할이지만, 윈도우 핸들 생략되어 있음 MFC가 숨겨 놓은 윈도우 핸들 자동 호출
	
	m_pMainWnd->ShowWindow(m_nCmdShow);
	//------------------------------------13번 연습 문제----------------------------
	//최소화 윈도우
	//m_pMainWnd->ShowWindow(SW_MINIMIZE);
	
	//초기화 작업 후 반드시 True 리턴 False 리턴 시 프로그램 종료
	return TRUE;
}

//메인 윈도우 클래스 정의
//생성자 호출 - 창을 만드는 역할
//CFrameWnd에서 파생한 CMainFrame은 메인 윈도우 기능
CMainFrame::CMainFrame()
{
	//윈도우 생성 SDK createWindow와 같은 역할
	//------------------------------------11번 연습 문제----------------------------
	//Create(NULL, _T("Hello World"));
	//Create(NULL, _T("HelloMFC"));
	//------------------------------------12번 연습 문제----------------------------
	//HelloMFC 예제에서 윈도우 스타일이 오버랩된 윈도우 대신 타이틀바(Caption)가 있는 
	//팝업 윈도우(Popup Window)가 되도록 수정하시오. 단, 윈도우 위치는 (0,0) 크기는 (500,200)
	CRect rect1(0, 0, 500, 200);
	Create(NULL, _T("HelloMFC"), WS_POPUPWINDOW|WS_CAPTION, rect1);

}

void CMainFrame::OnPaint()
{
	//SDK BeginPaint와 EndPaint처럼 CPaintDC객체를 만들면 생성자가 호출되고
	//MFC가 제공하는 BeginPaint 자동 호출 소멸자 호출 시 EndPaint 자동 호출
	CPaintDC dc(this);
	const TCHAR* msg = _T("Hello, MFC");
	//SDK TextOut과 동일한 역할
	dc.TextOut(100, 100, msg, lstrlen(msg));
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) {
	MessageBox(_T("마우스 클릭!"), _T("마우스 메시지"));
	//------------------------------------14번 연습 문제----------------------------
	//마우스 왼쪽 버튼 클릭시 윈도우창 최대화
	//ShowWindow(SW_MAXIMIZE);
	
	//------------------------------------15번 연습 문제----------------------------
	//마우스 왼쪽 버튼 클릭시 <예>/<아니오>/<취소> 버튼이 있는 메시지 상자 출력
	//MessageBox(_T("마우스 클릭!"), _T("마우스 메시지"), MB_YESNOCANCEL);

	//------------------------------------16번 연습 문제----------------------------
	//마우스 왼쪽 버튼 클릭시 물음표 아이콘이 있는 메시지 상자 출력
	//MessageBox(_T("마우스 클릭!"), _T("마우스 메시지"), MB_ICONQUESTION);

	//------------------------------------17번 연습 문제----------------------------
	//마우스 왼쪽 버튼 클릭시 <예>/<아니오> 버튼이 있는 메시지 상자 출력
	int result = MessageBox(_T("마우스 클릭!"), _T("마우스 메시지"), MB_YESNO);
	if (result == IDYES) {
		PostQuitMessage(0);
	}
}
void CMainFrame::OnRButtonDown(UINT nFlags, CPoint point) {
	MessageBox(_T("마우스 우클릭!"), _T("마우스 메시지"));
}


//메시지 맵 선언
//각 창은 메시지 맵이 무조건 있어야 함 = SDK 메시지 루프와 같은 역할
//WM_함수명 발생 시 ON_함수명 호출 - Mapping 관계 형성
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()