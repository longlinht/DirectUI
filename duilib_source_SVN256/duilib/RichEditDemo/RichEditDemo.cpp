// RichEditDemo.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "RichEditDemo.h"
#include "MainWindow.h"

Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
ULONG_PTR g_gdiplusToken;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	tstring strFileName = ZYM::CPath::GetAppPath() + _T("ImageOleCtrl.dll");
	BOOL bRet = DllRegisterServer(strFileName.c_str());	// ע��COM���
	if (!bRet)
	{
		::MessageBox(NULL, _T("COM���ע��ʧ�ܣ�Ӧ�ó����޷���ɳ�ʼ��������"), _T("��ʾ"), MB_OK);
		return 0;
	}

	HRESULT hr = ::OleInitialize(NULL);
	if (FAILED(hr))
		return 0;

	GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);	// ��ʼ��GDI+

	HMODULE hRichEditDll = ::LoadLibrary(_T("Riched20.dll"));	// ����RichEdit�ؼ�DLL

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	CMainWindow* pMainWindow = new CMainWindow();
	if (pMainWindow != NULL)
	{
		pMainWindow->Create(NULL, _T("QQ����Ի���"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 800);
		pMainWindow->SetIcon(IDI_RICHEDITDEMO);
		pMainWindow->CenterWindow();
		pMainWindow->ShowWindow();

		CPaintManagerUI::MessageLoop();
		delete pMainWindow;
	}

	CPaintManagerUI::Term();
	WindowImplBase::Cleanup();

	if (hRichEditDll != NULL)					// ж��RichEdit�ؼ�DLL
		::FreeLibrary(hRichEditDll);

	Gdiplus::GdiplusShutdown(g_gdiplusToken);	// ����ʼ��GDI+
	::OleUninitialize();

	return 0;
}