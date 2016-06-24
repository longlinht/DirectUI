#include "StdAfx.h"
#include "MainWindow.h"



MainWindow::MainWindow(void)
{
}

MainWindow::~MainWindow(void)
{
}

void MainWindow::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString MainWindow::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("skin\\");
#else
	return _T("skin\\");
#endif

}

DuiLib::CDuiString MainWindow::GetSkinFile()
{
	return _T("mainwin.xml");
}

UILIB_RESOURCETYPE MainWindow::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	return UILIB_ZIP;
#endif
}

DuiLib::CDuiString MainWindow::GetZIPFileName() const
{
	return _T("RichListRes.zip");
}

LPCTSTR MainWindow::GetWindowClassName(void) const
{
	return _T("MainWindow");
}


void MainWindow::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT MainWindow::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 解决ie控件收不到滚动消息的问题
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	::ScreenToClient(m_PaintManager.GetPaintWindow(), &pt);
	CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("ie")));
	if (pControl && pControl->IsVisible()) {
		RECT rc = pControl->GetPos();
		if (::PtInRect(&rc, pt)) {
			return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		}
	}

	bHandled = FALSE;
	return 0;
}

LRESULT MainWindow::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed) {
		if (!bZoomed) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(false);
		}
	}
	return lRes;
}

void MainWindow::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("minbtn")));
}

LRESULT MainWindow::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	/*演示键盘消息的处理*/
	TCHAR press_char = (TCHAR)wParam;
	if (press_char == VK_BACK)
	{
		MessageBox(NULL, _T("按下了回退键"), _T("DUILIB DEMO"), MB_OK);
	}
	else
	{
		bHandled = FALSE;
	}
	return 0;
}
