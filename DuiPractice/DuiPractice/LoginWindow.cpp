#include "StdAfx.h"
#include "LoginWindow.h"



LoginWindow::LoginWindow(void)
{
}

LoginWindow::~LoginWindow(void)
{
}

void LoginWindow::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString LoginWindow::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("skin\\");
#else
	return _T("skin\\");
#endif

}

DuiLib::CDuiString LoginWindow::GetSkinFile()
{
	return _T("loginwin.xml");
}

UILIB_RESOURCETYPE LoginWindow::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	return UILIB_ZIP;
#endif
}

DuiLib::CDuiString LoginWindow::GetZIPFileName() const
{
	return _T("RichListRes.zip");
}

LPCTSTR LoginWindow::GetWindowClassName(void) const
{
	return _T("LoginWindow");
}


void LoginWindow::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}


LRESULT LoginWindow::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// ��ʱ�����յ�WM_NCDESTROY���յ�wParamΪSC_CLOSE��WM_SYSCOMMAND
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

void LoginWindow::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("minbtn")));
}

LRESULT LoginWindow::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CControlUI* pHover = m_PaintManager.FindControl(pt);
	if (pHover == NULL) return 0;
	/*��ʾ��ͣ�������б��ͼ����ʱ����̬�任����ͼ��״̬��ʾ*/
	if (pHover->GetName() == _T("down_ico"))
	{
		MessageBox(NULL, _T("�����ĳ�ؼ����簴ť����ͣ�󣬶�Ŀ��ؼ�����������ı���״̬ͼ���С"), _T("DUILIB DEMO"), MB_OK);
		((CButtonUI *)pHover)->ApplyAttributeList(
			_T("normalimage=\"file='downlist_pause.png' dest='15,9,32,26'\""));
	}
	return 0;
}

LRESULT LoginWindow::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	TCHAR press_char = (TCHAR)wParam;
	if (press_char == VK_BACK)
	{
		MessageBox(NULL, _T("�����˻��˼�"), _T("DUILIB DEMO"), MB_OK);
	}
	else
	{
		bHandled = FALSE;
	}
	return 0;
}
