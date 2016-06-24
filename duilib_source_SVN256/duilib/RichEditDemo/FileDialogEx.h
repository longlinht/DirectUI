#pragma once

#include <commdlg.h>
#include <string>

struct __POSITION {};
typedef __POSITION* POSITION;

#if defined(UNICODE) || defined(_UNICODE)
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif

class CFileDialogEx
{
public:
	CFileDialogEx(void);
	~CFileDialogEx(void);

public:
	void SetDefExt(LPCTSTR lpszDefExt);
	void SetFileName(LPCTSTR lpszFileName);
	void SetFlags(DWORD dwFlags);
	void SetFilter(LPCTSTR lpszFilter);
	void SetMultiSel(BOOL bMultiSel = TRUE);
	void SetParentWnd(HWND hParentWnd);
	void SetTitle(LPCTSTR lpszTitle);
	void SetFileNameBufferSize(DWORD dwSize);

	BOOL ShowOpenFileDlg();
	BOOL ShowSaveFileDlg();

	tstring GetPathName();
	tstring GetFileName();
	tstring GetFileExt();
	tstring GetFileTitle();
	tstring GetFolderPath();

	POSITION GetStartPosition();
	tstring GetNextPathName(POSITION& pos);

public:
	OPENFILENAME m_stOFN;

private:
	TCHAR m_szDefExt[64];
	TCHAR m_szFilter[MAX_PATH];
	TCHAR m_szFileName[MAX_PATH];
	TCHAR * m_lpszFileName;
};
