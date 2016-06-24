// RichEditDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "RichEditDemo.h"
#include "MainWindow.h"

Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
ULONG_PTR g_gdiplusToken;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	tstring strFileName = ZYM::CPath::GetAppPath() + _T("ImageOleCtrl.dll");
	BOOL bRet = DllRegisterServer(strFileName.c_str());	// 注册COM组件
	if (!bRet)
	{
		::MessageBox(NULL, _T("COM组件注册失败，应用程序无法完成初始化操作！"), _T("提示"), MB_OK);
		return 0;
	}

	HRESULT hr = ::OleInitialize(NULL);
	if (FAILED(hr))
		return 0;

	GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);	// 初始化GDI+

	HMODULE hRichEditDll = ::LoadLibrary(_T("Riched20.dll"));	// 加载RichEdit控件DLL

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	CMainWindow* pMainWindow = new CMainWindow();
	if (pMainWindow != NULL)
	{
		pMainWindow->Create(NULL, _T("QQ聊天对话框"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 800);
		pMainWindow->SetIcon(IDI_RICHEDITDEMO);
		pMainWindow->CenterWindow();
		pMainWindow->ShowWindow();

		CPaintManagerUI::MessageLoop();
		delete pMainWindow;
	}

	CPaintManagerUI::Term();
	WindowImplBase::Cleanup();

	if (hRichEditDll != NULL)					// 卸载RichEdit控件DLL
		::FreeLibrary(hRichEditDll);

	Gdiplus::GdiplusShutdown(g_gdiplusToken);	// 反初始化GDI+
	::OleUninitialize();

	return 0;
}