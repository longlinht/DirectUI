#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include "MainWindow.h"
#include "LoginWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	MainWindow* pFrame = new MainWindow();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, 0L, 0, 0, 500, 400);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}
