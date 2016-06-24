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

class CFontInfo				// ������Ϣ
{
public:
	CFontInfo(void);
	~CFontInfo(void);

public:
	int m_nSize;			// �����С
	COLORREF m_clrText;		// ������ɫ
	tstring m_strName;		// ��������
	BOOL m_bBold;			// �Ƿ�Ӵ�
	BOOL m_bItalic;			// �Ƿ���б
	BOOL m_bUnderLine;		// �Ƿ���»���
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
	void OnMenu_Cut(TNotifyUI& msg);		// �����С��˵�
	void OnMenu_Copy(TNotifyUI& msg);		// �����ơ��˵�
	void OnMenu_Paste(TNotifyUI& msg);		// ��ճ�����˵�
	void OnMenu_SelAll(TNotifyUI& msg);		// ��ȫ��ѡ�񡱲˵�
	void OnMenu_Clear(TNotifyUI& msg);		// ���������˵�
	void OnMenu_ZoomRatio(TNotifyUI& msg);	// ����ʾ�������˵�
	void OnMenu_SaveAs(TNotifyUI& msg);		// �����Ϊ���˵�
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnFaceCtrlSel(UINT uMsg, WPARAM wParam, LPARAM lParam);	// �����顱�ؼ�ѡȡ��Ϣ

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
