
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "ClientDC.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CArray<CPoint, POINT&> carray;	

// CChildView

CChildView::CChildView()
{
	carray.SetSize(5);
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
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
	CPoint pos = carray.GetSize();
	for(int i =0; i<carray.GetSize(); i++){
		CPoint pt = carray[i];
		dc.Ellipse(pt.x - 20, pt.y - 20, pt.x + 20, pt.y + 20);
	}
}



void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnLButtonDown(nFlags, point);
	CClientDC dc(this);
	dc.Rectangle(point.x - 20, point.y - 20, point.x + 20, point.y + 20);
	CRect rect(point.x - 50, point.y - 20, point.x + 50, point.y + 20);
	dc.FillSolidRect(rect, RGB(255, 255, 0));
}


void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnRButtonDown(nFlags, point);
	CClientDC dc(this);
	dc.Ellipse(point.x - 20, point.y - 20, point.x + 20, point.y + 20);		//원 그리기
	carray.Add(point);														//배열에 원 포인트 저장
	CRect rect(point.x, point.y, point.x, point.y);
	dc.SetBkMode(TRANSPARENT);
	CString pos;
	pos.Format(_T("%d, %d"), point.x, point.y);
	dc.TextOut(point.x, point.y, pos);


}
