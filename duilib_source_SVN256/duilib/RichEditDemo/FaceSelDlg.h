#pragma once

#include "win_impl_base.h"
#include "FaceCtrl.h"

#define		FACE_CTRL_SEL		WM_USER + 1

class CFaceSelDlg : public WindowImplBase
{
public:
	CFaceSelDlg(void);
	~CFaceSelDlg(void);

public:
	LPCTSTR GetWindowClassName() const;	
	virtual void Init();
	virtual tString GetSkinFile();
	virtual tString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void OnFinalMessage(HWND hWnd);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	void SetFaceList(CFaceList * lpFaceList);
	int GetSelFaceId();
	int GetSelFaceIndex();
	tstring GetSelFaceFileName();

protected:
	void Notify(TNotifyUI& msg);

private:
	CFaceCtrl * m_pFaceCtrl;
	CFaceList * m_lpFaceList;
	int m_nSelFaceId;
	int m_nSelFaceIndex;
	tstring m_strSelFaceFileName;
};
