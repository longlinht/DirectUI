#include "StdAfx.h"
#include "MainWindow.h"

CFontInfo g_buddyFontInfo;

CFontInfo::CFontInfo(void)
{
	m_nSize = 9;
	m_clrText = RGB(0,0,0);
	m_strName = _T("宋体");
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_bUnderLine = FALSE;
}

CFontInfo::~CFontInfo(void)
{

}

CMainWindow::CMainWindow(void)
{
	m_pMinSysBtn = m_pMaxSysBtn = m_pRestoreSysBtn = m_pCloseSysBtn = NULL;
	m_pFontBtn = m_pFaceBtn = m_pImageBtn = NULL;
	m_pFontNameCombo = m_pFontSizeCombo = NULL;
	m_pBoldBtn = m_pItalicBtn = m_pUnderLineBtn = NULL;
	m_pColorBtn = NULL;
	m_pCloseBtn = m_pSendBtn = NULL;
	m_pSendEdit = m_pRecvEdit = NULL;

	memset(&m_ptRBtnDown, 0, sizeof(m_ptRBtnDown));
	m_strCheckMenu = _T("Menu_ZoomRatio_100");
	m_pLastImageOle = NULL;
	m_cxPicBarDlg = m_cyPicBarDlg = 0;
}

CMainWindow::~CMainWindow(void)
{
}

LPCTSTR CMainWindow::GetWindowClassName() const
{
	return _T("DUI_WINDOW");
}

void CMainWindow::Init()
{
	tstring strPath = ZYM::CPath::GetCurDir() + _T("Face\\FaceConfig.xml");
	m_FaceList.LoadConfigFile(strPath.c_str());

	m_pMinSysBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(_T("btnMinSys")));
	m_pMaxSysBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(_T("btnMaxSys")));
	m_pRestoreSysBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(_T("btnRestoreSys")));
	m_pCloseSysBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(_T("btnCloseSys")));
	m_pFontBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(_T("btnFont")));
	m_pFaceBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(_T("btnFace")));
	m_pImageBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(_T("btnImage")));
	m_pFontNameCombo = static_cast<CComboUI*>(paint_manager_.FindControl(_T("cboFontName")));
	m_pFontSizeCombo = static_cast<CComboUI*>(paint_manager_.FindControl(_T("cboFontSize")));
	m_pBoldBtn = static_cast<COptionUI*>(paint_manager_.FindControl(_T("btnBold")));
	m_pItalicBtn = static_cast<COptionUI*>(paint_manager_.FindControl(_T("btnItalic")));
	m_pUnderLineBtn = static_cast<COptionUI*>(paint_manager_.FindControl(_T("btnUnderLine")));
	m_pColorBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(_T("btnColor")));
	m_pCloseBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(_T("btnClose")));
	m_pSendBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(_T("btnSend")));
	m_pSendEdit = static_cast<CRichEditUI*>(paint_manager_.FindControl(_T("richSend")));
	m_pRecvEdit = static_cast<CRichEditUI*>(paint_manager_.FindControl(_T("richRecv")));

	DragAcceptFiles(m_hWnd, true);

	IRichEditOleCallback2* pRichEditOleCallback2 = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_ImageOle, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IRichEditOleCallback2), (void**)&pRichEditOleCallback2);
	if (SUCCEEDED(hr))
	{
		pRichEditOleCallback2->SetNotifyHwnd(m_hWnd);
		ITextServices * pTextServices = m_pRecvEdit->GetTextServices();
		pRichEditOleCallback2->SetTextServices(pTextServices);
		pTextServices->Release();
		m_pRecvEdit->SetOleCallback(pRichEditOleCallback2);
		pRichEditOleCallback2->Release();
	}

	pRichEditOleCallback2 = NULL;
	hr = ::CoCreateInstance(CLSID_ImageOle, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IRichEditOleCallback2), (void**)&pRichEditOleCallback2);
	if (SUCCEEDED(hr))
	{
		pRichEditOleCallback2->SetNotifyHwnd(m_hWnd);
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		pRichEditOleCallback2->SetTextServices(pTextServices);
		pTextServices->Release();
		m_pSendEdit->SetOleCallback(pRichEditOleCallback2);
		pRichEditOleCallback2->Release();
	}

	IDropTarget *pdt = m_pSendEdit->GetTxDropTarget();
	hr = ::RegisterDragDrop(m_hWnd, pdt);
	pdt->Release();
}

tString CMainWindow::GetSkinFile()
{
	return _T("MainWindow.xml");
}

tString CMainWindow::GetSkinFolder()
{
	return tString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

CControlUI* CMainWindow::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

LRESULT CMainWindow::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

LRESULT CMainWindow::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			if (m_pMaxSysBtn != NULL)
				m_pMaxSysBtn->SetVisible(false);
			if (m_pRestoreSysBtn != NULL)
				m_pRestoreSysBtn->SetVisible(true);
		}
		else 
		{
			if (m_pMaxSysBtn != NULL)
				m_pMaxSysBtn->SetVisible(true);
			if (m_pRestoreSysBtn != NULL)
				m_pRestoreSysBtn->SetVisible(false);
		}
	}

	return 0;
}

LRESULT CMainWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::RevokeDragDrop(m_hWnd);
	m_FaceList.Reset();
	bHandled = FALSE;
	return 0;
}

void CMainWindow::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

LRESULT CMainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_MOUSEMOVE)
		OnMouseMove(uMsg, wParam, lParam);

	if (uMsg == WM_RBUTTONDOWN)
	{
		LRESULT lRes = __super::HandleMessage(uMsg, wParam, lParam);
		OnRButtonDown(uMsg, wParam, lParam);
		return lRes;
	}

	if (uMsg == WM_LBUTTONDBLCLK)
		OnLButtonDblClk(uMsg, wParam, lParam);

 	if ((m_pSendEdit != NULL) && m_pSendEdit->IsFocused() 
 		&& (uMsg == WM_KEYDOWN) && (wParam == 'V') && (lParam & VK_CONTROL))	// 发送消息框的Ctrl+V消息
 	{
 		m_pSendEdit->PasteSpecial(CF_TEXT);
 		return TRUE;
 	}

	if (uMsg == WM_NOTIFY && EN_PASTE == ((LPNMHDR)lParam)->code)
	{
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		if ((UINT)pTextServices == ((LPNMHDR)lParam)->idFrom)
		{
			AddMsgToSendEdit(((NMRICHEDITOLECALLBACK *)lParam)->lpszText);
		}
		if (pTextServices != NULL)
			pTextServices->Release();
	}

	if (uMsg == WM_MENU)
	{
		CControlUI * pControl = (CControlUI *)lParam;
		if (pControl != NULL)
			this->paint_manager_.SendNotify(pControl, _T("menu_msg"), wParam, lParam);
	}

	if (uMsg == FACE_CTRL_SEL)
		return OnFaceCtrlSel(uMsg, wParam, lParam);

	if (uMsg == WM_DROPFILES)
		return onDropFiles(uMsg, wParam, lParam);

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainWindow::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

void CMainWindow::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_pMinSysBtn)
			OnBtn_Min(msg);
		else if (msg.pSender == m_pMaxSysBtn)
			OnBtn_Max(msg);
		else if (msg.pSender == m_pRestoreSysBtn)
			OnBtn_Restore(msg);
		else if (msg.pSender == m_pCloseSysBtn)
			OnBtn_Close(msg);
		else if (msg.pSender == m_pCloseBtn)
			OnBtn_Close(msg);
		else if (msg.pSender == m_pBoldBtn)
			OnBtn_Bold(msg);
		else if (msg.pSender == m_pItalicBtn)
			OnBtn_Italic(msg);
		else if (msg.pSender == m_pUnderLineBtn)
			OnBtn_UnderLine(msg);
		else if (msg.pSender == m_pColorBtn)
			OnBtn_Color(msg);
		else if (msg.pSender == m_pSendBtn)
			OnBtn_Send(msg);
		else if (msg.pSender == m_pFontBtn)
			OnBtn_Font(msg);
		else if (msg.pSender == m_pFaceBtn)
			OnBtn_Face(msg);
		else if (msg.pSender == m_pImageBtn)
			OnBtn_Image(msg);
	}
	else if (msg.sType == _T("itemselect"))
	{
		if (msg.pSender == m_pFontNameCombo)
			OnCbo_SelChange_FontName(msg);
		else if (msg.pSender == m_pFontSizeCombo)
			OnCbo_SelChange_FontSize(msg);
	}
	else if (msg.sType == _T("timer"))
		OnTimer(msg);
	else if (msg.sType == _T("menu_msg"))
	{
		CControlUI * pControl = (CControlUI *)msg.lParam;
		tstring strMenuName = (TCHAR *)(LPCTSTR)pControl->GetUserData();
		if (strMenuName == _T("Menu_Cut"))
			OnMenu_Cut(msg);
		else if (strMenuName == _T("Menu_Copy"))
			OnMenu_Copy(msg);
		else if (strMenuName == _T("Menu_Paste"))
			OnMenu_Paste(msg);
		else if (strMenuName == _T("Menu_SelAll"))
			OnMenu_SelAll(msg);
		else if (strMenuName == _T("Menu_Clear"))
			OnMenu_Clear(msg);
		else if (strMenuName == _T("Menu_ZoomRatio_400")
			|| strMenuName == _T("Menu_ZoomRatio_200")
			|| strMenuName == _T("Menu_ZoomRatio_150")
			|| strMenuName == _T("Menu_ZoomRatio_125")
			|| strMenuName == _T("Menu_ZoomRatio_100")
			|| strMenuName == _T("Menu_ZoomRatio_75")
			|| strMenuName == _T("Menu_ZoomRatio_50"))
			OnMenu_ZoomRatio(msg);
		else if (strMenuName == _T("Menu_SaveAs"))
			OnMenu_SaveAs(msg);
	}
}

void CMainWindow::OnBtn_Min(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void CMainWindow::OnBtn_Max(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void CMainWindow::OnBtn_Restore(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
}

void CMainWindow::OnBtn_Close(TNotifyUI& msg)
{
	Close();
	PostQuitMessage(0);
}

void CMainWindow::OnCbo_SelChange_FontName(TNotifyUI& msg)
{
	tstring strText = (TCHAR *)m_pFontNameCombo->GetText().GetData();
	if (m_fontInfo.m_strName != strText)
	{
		m_fontInfo.m_strName = strText;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_strName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_strName = m_fontInfo.m_strName;
	}
}

void CMainWindow::OnCbo_SelChange_FontSize(TNotifyUI& msg)
{
	tstring strText = (TCHAR *)m_pFontSizeCombo->GetText().GetData();

	int nSize = _tcstol(strText.c_str(), NULL, 10);
	if (m_fontInfo.m_nSize != nSize)
	{
		m_fontInfo.m_nSize = nSize;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_strName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_nSize = m_fontInfo.m_nSize;
	}
}

void CMainWindow::OnBtn_Bold(TNotifyUI& msg)
{
	BOOL bBold = !m_pBoldBtn->IsSelected();
	if (m_fontInfo.m_bBold != bBold)
	{
		m_fontInfo.m_bBold = bBold;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_strName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_bBold = m_fontInfo.m_bBold;
	}
}

void CMainWindow::OnBtn_Italic(TNotifyUI& msg)
{
	BOOL bItalic = !m_pItalicBtn->IsSelected();
	if (m_fontInfo.m_bItalic != bItalic)
	{
		m_fontInfo.m_bItalic = bItalic;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_strName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_bItalic = m_fontInfo.m_bItalic;
	}
}

void CMainWindow::OnBtn_UnderLine(TNotifyUI& msg)
{
	BOOL bUnderLine = !m_pUnderLineBtn->IsSelected();
	if (m_fontInfo.m_bUnderLine != bUnderLine)
	{
		m_fontInfo.m_bUnderLine = bUnderLine;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_strName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_bUnderLine = m_fontInfo.m_bUnderLine;
	}
}

void CMainWindow::OnBtn_Color(TNotifyUI& msg)
{
	static COLORREF rgbCustomColors[16] =
	{
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
	};

	CHOOSECOLOR cc = {0};
	cc.lStructSize = sizeof(cc);
	cc.lpCustColors = rgbCustomColors;
	cc.hwndOwner = m_hWnd;
	cc.Flags = CC_RGBINIT;

	BOOL bRet = ::ChooseColor(&cc);
	if (bRet)
	{
		COLORREF clrText = cc.rgbResult;
		if (m_fontInfo.m_clrText != clrText)
		{
			m_fontInfo.m_clrText = clrText;
			ITextServices * pTextServices = m_pSendEdit->GetTextServices();
			RichEdit_SetDefFont(pTextServices, m_fontInfo.m_strName.c_str(),
				m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
				m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
			pTextServices->Release();
			g_buddyFontInfo.m_clrText = m_fontInfo.m_clrText;
		}
	}
}

void CMainWindow::OnBtn_Font(TNotifyUI& msg)
{
	CContainerUI* pFontbar = static_cast<CContainerUI*>(paint_manager_.FindControl(_T("FontBar")));
	if (pFontbar != NULL)
		pFontbar->SetVisible(!pFontbar->IsVisible());
}

void CMainWindow::OnBtn_Face(TNotifyUI& msg)
{
	m_FaceSelDlg.SetFaceList(&m_FaceList);
	if (!::IsWindow((HWND)m_FaceSelDlg))
	{
		m_FaceSelDlg.Create(m_hWnd, NULL, WS_CHILD|WS_POPUP, WS_EX_TOOLWINDOW);

		RECT rcBtn = m_pFaceBtn->GetPos();
		::ClientToScreen(m_hWnd, (LPPOINT)&rcBtn);

		int cx = 432;
		int cy = 236;
		int x = rcBtn.left - cx / 2;
		int y = rcBtn.top - cy;

		::SetWindowPos((HWND)m_FaceSelDlg, NULL, x, y, cx, cy, NULL);
		::ShowWindow((HWND)m_FaceSelDlg, SW_SHOW);
	}
}

void CMainWindow::OnBtn_Image(TNotifyUI& msg)
{
	if (NULL == m_pSendEdit)
		return;

	LPCTSTR lpszFilter = _T("图像文件(*.bmp;*.jpg;*.jpeg;*.gif;*.png)\0*.bmp;*.jpg;*.jpeg;*.gif;*.png\0\0");;

	CFileDialogEx fileDlg;
	fileDlg.SetFilter(lpszFilter);
	fileDlg.SetParentWnd(m_hWnd);
	if (fileDlg.ShowOpenFileDlg())
	{
		_RichEdit_InsertFace(m_pSendEdit, fileDlg.GetPathName().c_str(), -1, -1);
		m_pSendEdit->SetFocus();
	}
}

void CMainWindow::OnBtn_Send(TNotifyUI& msg)
{
	ITextServices * pTextServices = m_pSendEdit->GetTextServices();

	tstring strText;
	RichEdit_GetText(pTextServices, strText);

	pTextServices->Release();

	if (strText.size() <= 0)
		return;

	m_pSendEdit->SetText(_T(""));
	m_pSendEdit->SetFocus();

	AddMsgToRecvEdit(strText.c_str());
}

void CMainWindow::OnTimer(TNotifyUI& msg)
{
}

// “剪切”菜单
void CMainWindow::OnMenu_Cut(TNotifyUI& msg)
{
	m_pSendEdit->Cut();
}

// “复制”菜单
void CMainWindow::OnMenu_Copy(TNotifyUI& msg)
{
	if (msg.pSender == m_pSendEdit)
		m_pSendEdit->Copy();
	else if (msg.pSender == m_pRecvEdit)
		m_pRecvEdit->Copy();
}

// “粘贴”菜单
void CMainWindow::OnMenu_Paste(TNotifyUI& msg)
{
	m_pSendEdit->PasteSpecial(CF_TEXT);
}

// “全部选择”菜单
void CMainWindow::OnMenu_SelAll(TNotifyUI& msg)
{
	if (msg.pSender == m_pSendEdit)
		m_pSendEdit->SetSel(0, -1);
	else if (msg.pSender == m_pRecvEdit)
		m_pRecvEdit->SetSel(0, -1);
}

// “清屏”菜单
void CMainWindow::OnMenu_Clear(TNotifyUI& msg)
{
	m_pRecvEdit->SetText(_T(""));
}

// “显示比例”菜单
void CMainWindow::OnMenu_ZoomRatio(TNotifyUI& msg)
{
	CControlUI * pControl = (CControlUI *)msg.lParam;
	tstring strMenuName = (TCHAR *)(LPCTSTR)pControl->GetUserData();
	if (strMenuName == _T("Menu_ZoomRatio_400"))
		m_pRecvEdit->SetZoom(16, 4);
	else if (strMenuName == _T("Menu_ZoomRatio_200"))
		m_pRecvEdit->SetZoom(8, 4);
	else if (strMenuName == _T("Menu_ZoomRatio_150"))
		m_pRecvEdit->SetZoom(6, 4);
	else if (strMenuName == _T("Menu_ZoomRatio_125"))
		m_pRecvEdit->SetZoom(5, 4);
	else if (strMenuName == _T("Menu_ZoomRatio_100"))
		m_pRecvEdit->SetZoom(0, 0);
	else if (strMenuName == _T("Menu_ZoomRatio_75"))
		m_pRecvEdit->SetZoom(3, 4);
	else if (strMenuName == _T("Menu_ZoomRatio_50"))
		m_pRecvEdit->SetZoom(1, 2);
	else
		return;

	m_strCheckMenu = strMenuName;
}

// “另存为”菜单
void CMainWindow::OnMenu_SaveAs(TNotifyUI& msg)
{
	if (msg.pSender != m_pSendEdit && msg.pSender != m_pRecvEdit)
		return;
	
	ITextServices * pTextServices = ((CRichEditUI *)msg.pSender)->GetTextServices();
	IImageOle * pImageOle = NULL;
	BOOL bRet = RichEdit_GetImageOle(pTextServices, m_ptRBtnDown, &pImageOle);
	pTextServices->Release();
	if (!bRet || NULL == pImageOle)
		return;

	tstring strFileName;

	BSTR bstrFileName = NULL;
	HRESULT hr = pImageOle->GetFileName(&bstrFileName);
	if (SUCCEEDED(hr))
		strFileName = bstrFileName;
	if (bstrFileName != NULL)
		::SysFreeString(bstrFileName);

	LPCTSTR lpszDefExt;
	LPCTSTR lpszFileName = _T("未命名");
	DWORD dwFlags = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR|OFN_EXTENSIONDIFFERENT;
	LPCTSTR lpszFilter;
	HWND hWndParent = m_hWnd;

	GUID guid = {0};
	hr = pImageOle->GetRawFormat(&guid);

	if (InlineIsEqualGUID(guid, Gdiplus::ImageFormatJPEG))
	{
		lpszDefExt = _T(".jpg");
		lpszFilter = _T("图像文件(*.jpg)\0*.jpg\0图像文件(*.bmp)\0*.bmp\0\0");
	}
	else if (InlineIsEqualGUID(guid, Gdiplus::ImageFormatPNG))
	{
		lpszDefExt = _T(".png");
		lpszFilter = _T("图像文件(*.png)\0*.png\0\0");
	}
	else if (InlineIsEqualGUID(guid, Gdiplus::ImageFormatGIF))
	{
		lpszDefExt = _T(".gif");
		lpszFilter = _T("图像文件(*.gif)\0*.gif\0图像文件(*.jpg)\0*.jpg\0图像文件(*.bmp)\0*.bmp\0\0");
	}
	else
	{
		lpszDefExt = _T(".jpg");
		lpszFilter = _T("图像文件(*.jpg)\0*.jpg\0图像文件(*.bmp)\0*.bmp\0\0");
	}

	CFileDialogEx fileDlg;
	fileDlg.SetDefExt(lpszDefExt);
	fileDlg.SetFileName(lpszFileName);
	fileDlg.SetFlags(dwFlags);
	fileDlg.SetFilter(lpszFilter);
	fileDlg.SetParentWnd(m_hWnd);
	fileDlg.SetTitle(_T("保存图片"));
	if (fileDlg.ShowSaveFileDlg())
	{
		tstring strSavePath = fileDlg.GetPathName();
		tstring strExtName = _T(".") + ZYM::CPath::GetExtension(strSavePath.c_str());
		GUID guid2 = GetFileTypeGuidByExtension(strExtName.c_str());

		if (InlineIsEqualGUID(guid, guid2))
		{
			CopyFile(strFileName.c_str(), strSavePath.c_str(), FALSE);
		}
		else
		{
			BSTR bstrSavePath = ::SysAllocString(strSavePath.c_str());
			if (bstrSavePath != NULL)
			{
				pImageOle->SaveAsFile(bstrSavePath);
				::SysFreeString(bstrSavePath);
			}
		}
	}

	if (pImageOle != NULL)
		pImageOle->Release();
}

LRESULT CMainWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	IImageOle * pNewImageOle = NULL;
	RECT rc = {0};

	POINT pt = {LOWORD(lParam), HIWORD(lParam)};
	CControlUI * pControl = this->paint_manager_.FindControl(pt);
	if (pControl != NULL)
	{
		if (pControl == m_pSendEdit || pControl == m_pRecvEdit)
		{
			ITextServices * pTextServices = ((CRichEditUI *)pControl)->GetTextServices();

			IImageOle * pImageOle = NULL;
			BOOL bRet = RichEdit_GetImageOle(pTextServices, pt, &pImageOle);
			if (bRet && pImageOle != NULL)
			{
				pNewImageOle = pImageOle;
				pImageOle->GetObjectRect(&rc);
			}
			if (pImageOle != NULL)
				pImageOle->Release();

			pTextServices->Release();
		}
	}

	if (m_pLastImageOle != pNewImageOle)
	{
		m_pLastImageOle = pNewImageOle;
		if (m_pLastImageOle != NULL)
		{
			m_ptRBtnDown = pt;

			if (!::IsWindow((HWND)m_PicBarDlg))
			{
				m_PicBarDlg.Create(m_hWnd, NULL, WS_CHILD|WS_POPUP, WS_EX_TOOLWINDOW);

				RECT rcPos = {0};
				::GetClientRect((HWND)m_PicBarDlg, &rcPos);
				m_cxPicBarDlg = rcPos.right-rcPos.left;
				m_cyPicBarDlg = rcPos.bottom-rcPos.top;
			}

			m_PicBarDlg.SetCallBackCtrl(pControl);

			RECT rc2 = pControl->GetPos();
			POINT pt = {rc.right, rc.bottom-m_cyPicBarDlg};
			if (pt.x < rc2.left)
				pt.x = rc2.left;
			if (pt.x > rc2.right)
				pt.x = rc2.right;
 			if (pt.y > rc2.bottom-m_cyPicBarDlg)
				pt.y = rc2.bottom-m_cyPicBarDlg;
			::ClientToScreen(m_hWnd, &pt);

			::SetWindowPos((HWND)m_PicBarDlg, NULL, pt.x, pt.y, m_cxPicBarDlg, m_cyPicBarDlg, SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}
		else
		{
			::ShowWindow((HWND)m_PicBarDlg, SW_HIDE);
		}
	}

	return 0;
}

LRESULT CMainWindow::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CControlUI * pControl = this->paint_manager_.FindControl(pt);
	if (pControl != NULL)
	{	
		if (pControl == m_pRecvEdit)
		{
			m_ptRBtnDown = pt;

			CMenuWnd* pMenu = new CMenuWnd(m_hWnd);
			::ClientToScreen(m_hWnd, &pt);
			pMenu->Init(NULL, _T("RecvEditMenu.xml"), _T("xml"), pt, pControl);

			BOOL bSel = (m_pRecvEdit->GetSelectionType() != SEL_EMPTY);
			pMenu->EnableMenuItem(_T("Menu_Copy"), bSel);

			pMenu->CheckMenuItem(m_strCheckMenu.c_str(), TRUE);

			ITextServices * pTextServices = m_pRecvEdit->GetTextServices();

			IImageOle * pImageOle = NULL;
			BOOL bRet = RichEdit_GetImageOle(pTextServices, m_ptRBtnDown, &pImageOle);
			BOOL bSaveAs = (bRet && pImageOle != NULL);
			pMenu->EnableMenuItem(_T("Menu_SaveAs"), bSaveAs);
			if (pImageOle != NULL)
				pImageOle->Release();

			pTextServices->Release();
		}
		else if (pControl == m_pSendEdit)
		{
			m_ptRBtnDown = pt;

			CMenuWnd* pMenu = new CMenuWnd(m_hWnd);
			::ClientToScreen(m_hWnd, &pt);
			pMenu->Init(NULL, _T("SendEditMenu.xml"), _T("xml"), pt, pControl);

			BOOL bSel = (m_pSendEdit->GetSelectionType() != SEL_EMPTY);
			pMenu->EnableMenuItem(_T("Menu_Cut"), bSel);
			pMenu->EnableMenuItem(_T("Menu_Copy"), bSel);

			BOOL bPaste = m_pSendEdit->CanPaste();
			pMenu->EnableMenuItem(_T("Menu_Paste"), bPaste);

			ITextServices * pTextServices = m_pSendEdit->GetTextServices();

			IImageOle * pImageOle = NULL;
			BOOL bRet = RichEdit_GetImageOle(pTextServices, m_ptRBtnDown, &pImageOle);
			BOOL bSaveAs = (bRet && pImageOle != NULL);
			pMenu->EnableMenuItem(_T("Menu_SaveAs"), bSaveAs);
			if (pImageOle != NULL)
				pImageOle->Release();

			pTextServices->Release();
		}
	}
	return 0;
}

LRESULT CMainWindow::OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	CControlUI * pControl = this->paint_manager_.FindControl(pt);
	if (pControl != NULL)
	{
		if (pControl == m_pSendEdit || pControl == m_pRecvEdit)
		{
			ITextServices * pTextServices = ((CRichEditUI *)pControl)->GetTextServices();

			IImageOle * pImageOle = NULL;
			BOOL bRet = RichEdit_GetImageOle(pTextServices, pt, &pImageOle);
			if (bRet && pImageOle != NULL)
			{
				BSTR bstrFileName = NULL;
				HRESULT hr = pImageOle->GetFileName(&bstrFileName);
				if (SUCCEEDED(hr))
					::ShellExecute(NULL, _T("open"), bstrFileName, NULL, NULL, SW_SHOWNORMAL);
				if (bstrFileName != NULL)
					::SysFreeString(bstrFileName);
			}
			if (pImageOle != NULL)
				pImageOle->Release();

			pTextServices->Release();
		}
	}
	return 0;
}

// “表情”控件选取消息
LRESULT CMainWindow::OnFaceCtrlSel(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nFaceId = m_FaceSelDlg.GetSelFaceId();
	int nFaceIndex = m_FaceSelDlg.GetSelFaceIndex();
	tstring strFileName = m_FaceSelDlg.GetSelFaceFileName();
	if (!strFileName.empty())
	{
		_RichEdit_InsertFace(m_pSendEdit, strFileName.c_str(), nFaceId, nFaceIndex);
		m_pSendEdit->SetFocus();
	}
	((COptionUI *)m_pFaceBtn)->Selected(false);
	return 0;
}

LRESULT CMainWindow::onDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	TCHAR acFile[MAX_PATH + 1];

	DragQueryFile((HDROP)wParam, 0, acFile, sizeof(acFile));

	MessageBox(NULL, acFile, _T("Some dropped:"), MB_OK);

	DragFinish((HDROP)wParam);

	_RichEdit_InsertFace(m_pSendEdit, acFile, -1, -1);
	m_pSendEdit->SetFocus();

	return 0;
}

void CMainWindow::_RichEdit_ReplaceSel(CRichEditUI * pRichEdit, LPCTSTR lpszNewText)
{
	ITextServices * pTextServices = pRichEdit->GetTextServices();
	if (pRichEdit == m_pRecvEdit)
	{
		RichEdit_ReplaceSel(pTextServices, lpszNewText, 
			m_fontInfo.m_strName.c_str(), m_fontInfo.m_nSize, 
			m_fontInfo.m_clrText, m_fontInfo.m_bBold, m_fontInfo.m_bItalic, 
			m_fontInfo.m_bUnderLine, FALSE, 300);
	}
	else
	{
		RichEdit_ReplaceSel(pTextServices, lpszNewText);
	}
	pTextServices->Release();
}

BOOL CMainWindow::_RichEdit_InsertFace(CRichEditUI * pRichEdit, LPCTSTR lpszFileName, int nFaceId, int nFaceIndex)
{
	BOOL bRet = FALSE;

	if (NULL == pRichEdit || NULL == lpszFileName || NULL == *lpszFileName)
		return FALSE;

	ITextServices * pTextServices = pRichEdit->GetTextServices();
	ITextHost * pTextHost = pRichEdit->GetTextHost();
	if (pTextServices != NULL && pTextHost != NULL)
	{
		if (pRichEdit == m_pRecvEdit)
			RichEdit_SetStartIndent(pTextServices, 300);
		bRet = RichEdit_InsertFace(pTextServices, pTextHost, 
			lpszFileName, nFaceId, nFaceIndex, RGB(255,255,255), TRUE, 40);
	}
	
	if (pTextServices != NULL)
		pTextServices->Release();
	if (pTextHost != NULL)
		pTextHost->Release();

	return bRet;
}

BOOL CMainWindow::HandleSysFaceId(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText)
{
	int nFaceId = GetBetweenInt(p+2, _T("[\""), _T("\"]"), -1);
	CFaceInfo * lpFaceInfo = m_FaceList.GetFaceInfoById(nFaceId);
	if (lpFaceInfo != NULL)
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel(pRichEdit, strText.c_str()); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), 
			lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}

BOOL CMainWindow::HandleSysFaceIndex(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText)
{
	int nFaceIndex = GetBetweenInt(p+2, _T("[\""), _T("\"]"), -1);
	CFaceInfo * lpFaceInfo = m_FaceList.GetFaceInfoByIndex(nFaceIndex);
	if (lpFaceInfo != NULL)
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel(pRichEdit, strText.c_str()); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), 
			lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}

BOOL CMainWindow::HandleCustomPic(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText)
{
	tstring strFileName = GetBetweenString(p+2, _T("[\""), _T("\"]"));
	if (!strFileName.empty())
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel(pRichEdit, strText.c_str()); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, strFileName.c_str(), -1, -1);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}

// "/f["系统表情id"]/s["系统表情index"]/c["自定义图片路径"]"
void CMainWindow::AddMsg(CRichEditUI * pRichEdit, LPCTSTR lpText)
{
	if (NULL == pRichEdit || NULL == lpText || NULL == *lpText)
		return;

	tstring strText;

	for (LPCTSTR p = lpText; *p != _T('\0'); p++)
	{
		if (*p == _T('/'))
		{
			if (*(p+1) == _T('/'))
			{
				strText += *p;
				p++;
				continue;
			}
			else if (*(p+1) == _T('f'))
			{
				if (HandleSysFaceId(pRichEdit, p, strText))
					continue;
			}
			else if (*(p+1) == _T('s'))
			{
				if (HandleSysFaceIndex(pRichEdit, p, strText))
					continue;
			}
			else if (*(p+1) == _T('c'))
			{
				if (HandleCustomPic(pRichEdit, p, strText))
					continue;
			}
		}
		strText += *p;
	}

	if (!strText.empty())
		_RichEdit_ReplaceSel(pRichEdit, strText.c_str());
}

void CMainWindow::AddMsgToSendEdit(LPCTSTR lpText)
{
	AddMsg(m_pSendEdit, lpText);
	m_pSendEdit->EndDown();
}

void CMainWindow::AddMsgToRecvEdit(LPCTSTR lpText)
{
	if (NULL == lpText || NULL == *lpText)
		return;

	m_pRecvEdit->SetAutoURLDetect(true);

	tstring strTime;
	strTime = FormatTime(time(NULL), _T("%H:%M:%S"));

	ITextServices * pTextServices = m_pRecvEdit->GetTextServices();
	RichEdit_SetSel(pTextServices, -1, -1);

	TCHAR cText[2048] = {0};
	wsprintf(cText, _T("%s("), _T("倚天"));

	RichEdit_ReplaceSel(pTextServices, cText, 
		_T("宋体"), 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);

	wsprintf(cText, _T("%u"), 43156150);
	RichEdit_ReplaceSel(pTextServices, cText, 
		_T("宋体"), 9, RGB(0, 114, 193), FALSE, FALSE, TRUE, TRUE, 0);

	wsprintf(cText, _T(")  %s\r\n"), strTime.c_str());
	RichEdit_ReplaceSel(pTextServices, cText, 
		_T("宋体"), 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);

	//m_pRecvEdit->SetAutoURLDetect(true);

	AddMsg(m_pRecvEdit, lpText);

	RichEdit_ReplaceSel(pTextServices, _T("\r\n"));
	RichEdit_SetStartIndent(pTextServices, 0);
	m_pRecvEdit->EndDown();

	pTextServices->Release();
}