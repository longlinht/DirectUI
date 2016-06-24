#pragma once

#include "win_impl_base.h"
#include "IImageOle.h"
#include "RichEditUtil.h"
#include "FileDialogEx.h"
#include "Path.h"
#include "FaceList.h"
#include "UIMenu.h"
#include "FaceSelDlg.h"
#include "PicBarDlg.h"
#include <shellapi.h>

class CFontInfo				// 字体信息
{
public:
	CFontInfo(void);
	~CFontInfo(void);

public:
	int m_nSize;			// 字体大小
	COLORREF m_clrText;		// 字体颜色
	tstring m_strName;		// 字体名称
	BOOL m_bBold;			// 是否加粗
	BOOL m_bItalic;			// 是否倾斜
	BOOL m_bUnderLine;		// 是否带下划线
};

class CMainWindow : public WindowImplBase
{
public:
	CMainWindow(void);
	~CMainWindow(void);

	LPCTSTR GetWindowClassName() const;	
	virtual void Init();
	virtual tString GetSkinFile();
	virtual tString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void OnFinalMessage(HWND hWnd);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	void Notify(TNotifyUI& msg);

	void OnBtn_Min(TNotifyUI& msg);
	void OnBtn_Max(TNotifyUI& msg);
	void OnBtn_Restore(TNotifyUI& msg);
	void OnBtn_Close(TNotifyUI& msg);
	void OnCbo_SelChange_FontName(TNotifyUI& msg);
	void OnCbo_SelChange_FontSize(TNotifyUI& msg);
	void OnBtn_Bold(TNotifyUI& msg);
	void OnBtn_Italic(TNotifyUI& msg);
	void OnBtn_UnderLine(TNotifyUI& msg);
	void OnBtn_Color(TNotifyUI& msg);
	void OnBtn_Font(TNotifyUI& msg);
	void OnBtn_Face(TNotifyUI& msg);
	void OnBtn_Image(TNotifyUI& msg);
	void OnBtn_Send(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
	void OnMenu_Cut(TNotifyUI& msg);		// “剪切”菜单
	void OnMenu_Copy(TNotifyUI& msg);		// “复制”菜单
	void OnMenu_Paste(TNotifyUI& msg);		// “粘贴”菜单
	void OnMenu_SelAll(TNotifyUI& msg);		// “全部选择”菜单
	void OnMenu_Clear(TNotifyUI& msg);		// “清屏”菜单
	void OnMenu_ZoomRatio(TNotifyUI& msg);	// “显示比例”菜单
	void OnMenu_SaveAs(TNotifyUI& msg);		// “另存为”菜单
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnFaceCtrlSel(UINT uMsg, WPARAM wParam, LPARAM lParam);	// “表情”控件选取消息

	LRESULT onDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void _RichEdit_ReplaceSel(CRichEditUI * pRichEdit, LPCTSTR lpszNewText);
	BOOL _RichEdit_InsertFace(CRichEditUI * pRichEdit, LPCTSTR lpszFileName, int nFaceId, int nFaceIndex);
	BOOL HandleSysFaceId(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);
	BOOL HandleSysFaceIndex(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);
	BOOL HandleCustomPic(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);
	void AddMsg(CRichEditUI * pRichEdit, LPCTSTR lpText);
	void AddMsgToSendEdit(LPCTSTR lpText);
	void AddMsgToRecvEdit(LPCTSTR lpText);

private:
	CButtonUI * m_pMinSysBtn, * m_pMaxSysBtn, * m_pRestoreSysBtn, * m_pCloseSysBtn;
	CButtonUI * m_pFontBtn, * m_pFaceBtn, * m_pImageBtn;
	CComboUI * m_pFontNameCombo, * m_pFontSizeCombo;
	COptionUI * m_pBoldBtn, * m_pItalicBtn, * m_pUnderLineBtn;
	CButtonUI * m_pColorBtn;
	CButtonUI * m_pCloseBtn, * m_pSendBtn;
	CRichEditUI* m_pSendEdit, * m_pRecvEdit;

	CFaceSelDlg m_FaceSelDlg;
	CPicBarDlg m_PicBarDlg;

	CFaceList m_FaceList;
	CFontInfo m_fontInfo;
	POINT m_ptRBtnDown;
	tstring m_strCheckMenu;
	IImageOle * m_pLastImageOle;
	int m_cxPicBarDlg, m_cyPicBarDlg;
};
