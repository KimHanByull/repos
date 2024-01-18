//��� ���� ����
#include <afxwin.h>

//���� ���α׷� Ŭ���� ����
//���� ���α׷��� �ʱ�ȭ, �޽��� ����, ���� �ڵ� ����
class CHelloApp : public CWinApp 
{
public:
	virtual BOOL InitInstance();
};

//���� ������ Ŭ���� ����
//���� ������ ��� ����
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

//���� ���α׷� ��ü�� ����
//CWinApp Ŭ������ ��ӹ޾� Ŭ������ ����� �� Ŭ������ ��ü �ϳ��� ���� ����(���� ���α׷� ��ü)�� ����(�ϳ��� ���� ����)
CHelloApp theApp;

//���� ���α׷� Ŭ������ ����
//�ν��Ͻ� �ʱ�ȭ - �������α׷� �ʱ�ȭ -> ������ �ϳ� ���� -> ȭ�鿡 ���� ����
//main �Լ� ���� ����Ǿ� �򰥸� (���ο� ������ �ִ� �κ�)
//InitInstance �Լ� ȣ��� ������ �ʼ�
//��ӹ޾� ����ϴ� �����Լ��� �ſ� �߿�
BOOL CHelloApp::InitInstance()
{
	m_pMainWnd = new CMainFrame;
	//SDK ShowWindow�� ���� ����������, ������ �ڵ� �����Ǿ� ���� MFC�� ���� ���� ������ �ڵ� �ڵ� ȣ��
	
	m_pMainWnd->ShowWindow(m_nCmdShow);
	//------------------------------------13�� ���� ����----------------------------
	//�ּ�ȭ ������
	//m_pMainWnd->ShowWindow(SW_MINIMIZE);
	
	//�ʱ�ȭ �۾� �� �ݵ�� True ���� False ���� �� ���α׷� ����
	return TRUE;
}

//���� ������ Ŭ���� ����
//������ ȣ�� - â�� ����� ����
//CFrameWnd���� �Ļ��� CMainFrame�� ���� ������ ���
CMainFrame::CMainFrame()
{
	//������ ���� SDK createWindow�� ���� ����
	//------------------------------------11�� ���� ����----------------------------
	//Create(NULL, _T("Hello World"));
	//Create(NULL, _T("HelloMFC"));
	//------------------------------------12�� ���� ����----------------------------
	//HelloMFC �������� ������ ��Ÿ���� �������� ������ ��� Ÿ��Ʋ��(Caption)�� �ִ� 
	//�˾� ������(Popup Window)�� �ǵ��� �����Ͻÿ�. ��, ������ ��ġ�� (0,0) ũ��� (500,200)
	CRect rect1(0, 0, 500, 200);
	Create(NULL, _T("HelloMFC"), WS_POPUPWINDOW|WS_CAPTION, rect1);

}

void CMainFrame::OnPaint()
{
	//SDK BeginPaint�� EndPaintó�� CPaintDC��ü�� ����� �����ڰ� ȣ��ǰ�
	//MFC�� �����ϴ� BeginPaint �ڵ� ȣ�� �Ҹ��� ȣ�� �� EndPaint �ڵ� ȣ��
	CPaintDC dc(this);
	const TCHAR* msg = _T("Hello, MFC");
	//SDK TextOut�� ������ ����
	dc.TextOut(100, 100, msg, lstrlen(msg));
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) {
	MessageBox(_T("���콺 Ŭ��!"), _T("���콺 �޽���"));
	//------------------------------------14�� ���� ����----------------------------
	//���콺 ���� ��ư Ŭ���� ������â �ִ�ȭ
	//ShowWindow(SW_MAXIMIZE);
	
	//------------------------------------15�� ���� ����----------------------------
	//���콺 ���� ��ư Ŭ���� <��>/<�ƴϿ�>/<���> ��ư�� �ִ� �޽��� ���� ���
	//MessageBox(_T("���콺 Ŭ��!"), _T("���콺 �޽���"), MB_YESNOCANCEL);

	//------------------------------------16�� ���� ����----------------------------
	//���콺 ���� ��ư Ŭ���� ����ǥ �������� �ִ� �޽��� ���� ���
	//MessageBox(_T("���콺 Ŭ��!"), _T("���콺 �޽���"), MB_ICONQUESTION);

	//------------------------------------17�� ���� ����----------------------------
	//���콺 ���� ��ư Ŭ���� <��>/<�ƴϿ�> ��ư�� �ִ� �޽��� ���� ���
	int result = MessageBox(_T("���콺 Ŭ��!"), _T("���콺 �޽���"), MB_YESNO);
	if (result == IDYES) {
		PostQuitMessage(0);
	}
}
void CMainFrame::OnRButtonDown(UINT nFlags, CPoint point) {
	MessageBox(_T("���콺 ��Ŭ��!"), _T("���콺 �޽���"));
}


//�޽��� �� ����
//�� â�� �޽��� ���� ������ �־�� �� = SDK �޽��� ������ ���� ����
//WM_�Լ��� �߻� �� ON_�Լ��� ȣ�� - Mapping ���� ����
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()