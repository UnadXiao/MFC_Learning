
#include <afxwin.h>


// Ӧ�ó����ඨ��
class COurApp : public CWinApp
{
public:
	virtual BOOL InitInstance() override;
};

// �����ඨ��
class COurWnd : public CFrameWnd
{
public:
	COurWnd()
	{
		Create(nullptr, _T("Our Dumb MFC Application"));
	}
};

// ����������ʵ��
BOOL COurApp::InitInstance(void)
{
	m_pMainWnd = new COurWnd();
	m_pMainWnd->ShowWindow(m_nCmdShow);
	return TRUE;
}

// ȫ�ֶ���Ӧ�ó������
COurApp AnApplication;