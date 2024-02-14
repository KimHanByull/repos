
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "DrawCircles.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_bDrawMode = FALSE;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}



void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//마우스 캡쳐 시작(MFC 함수 사용)
	// - 마우스 커서의 위치와 관계없이 모든 마우스 메시지가 특정 윈도우에 전달되게 하는 메커니즘
	
	SetCapture();

	CWnd::OnLButtonDown(nFlags, point);
	//그리기 모드를 시작한다.
	m_bDrawMode = TRUE;
	//좌표 지정
	m_x1 = m_x2 = point.x;
	m_y1 = m_y2 = point.y;
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnLButtonUp(nFlags, point);
	CClientDC dc(this);
	dc.SelectStockObject(NULL_BRUSH);
	//최종 타원을 그림
	dc.SetROP2(R2_COPYPEN);
	m_x2 = point.x;
	m_y2 = point.y;
	dc.Ellipse(m_x1, m_y1, m_x2, m_y2);
	//그리기 모드 종류
	m_bDrawMode = FALSE;

	//마우스 캡처 해제(API 함수 사용)
	::ReleaseCapture();
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnMouseMove(nFlags, point);
	//그리기 모드이면 타원을 지우고 그리기를 반복
	if (m_bDrawMode) {
		CClientDC dc(this);
		dc.SelectStockObject(NULL_BRUSH);
		//이전에 그린 타원을 지운다.
		dc.SetROP2(R2_NOT);
		dc.Ellipse(m_x1, m_y1, m_x2, m_y2);
		//새로운 타원 생성
		dc.SetROP2(R2_NOT);
		m_x2 = point.x;
		m_y2 = point.y;
		dc.Ellipse(m_x1, m_y1, m_x2, m_y2);
	}
}
