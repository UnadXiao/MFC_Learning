
#include <afxwin.h>


// 应用程序类定义
class COurApp : public CWinApp
{
public:
	virtual BOOL InitInstance() override;
};

// 窗口类定义
class COurWnd : public CFrameWnd
{
public:
	COurWnd()
	{
		Create(nullptr, _T("Our Dumb MFC Application"));
	}
};

// 创建主窗口实例
BOOL COurApp::InitInstance(void)
{
	m_pMainWnd = new COurWnd();
	m_pMainWnd->ShowWindow(m_nCmdShow);
	return TRUE;
}

// 全局定义应用程序对象
COurApp AnApplication;