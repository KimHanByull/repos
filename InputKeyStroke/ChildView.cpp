
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "InputKeyStroke.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_xpos = m_ypos = 60;	//임의 값으로 초기화
	m_bFill = FALSE;		//도형 내부 채우지 않음
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
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
	if (m_bFill == TRUE)dc.SelectStockObject(BLACK_BRUSH);
	dc.Ellipse(m_xpos - 20, m_ypos - 20, m_xpos + 20, m_ypos + 20);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}



void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	m_xMax = cx;
	m_yMax = cy;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar) {
	case VK_LEFT:
		m_xpos -= 50;
		break;
	case VK_RIGHT:
		m_xpos += 50;
		break;
	case VK_UP:
		m_ypos -= 50;
		break;
	case VK_DOWN:
		m_ypos += 50;
		break;

	case VK_SPACE:
		m_bFill = !m_bFill;
	}

	//20 <= m_xpos <= m_xMax - 20;	원이 윈도우 밖으로 나가는 현상 방지 목적(원의 끝점과 윈도우 영역 끝 내접)
	m_xpos = min(max(50, m_xpos), m_xMax - 50);
	//20 <= m_ypos <= m_yMax - 20;	원이 윈도우 밖으로 나가는 현상 방지 목적(원의 끝점과 윈도우 영역 끝 내접)
	m_ypos = min(max(50, m_ypos), m_yMax - 50);
	Invalidate();
	

}	
