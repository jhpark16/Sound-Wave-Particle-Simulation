// Particle_Simulation.cpp : main source file for Particle_Simulation.exe
//

#include "stdafx.h"

#include "resource.h"

#include "View.h"
#include "aboutdlg.h"
#include "MainFrm.h"

CAppModule _Module;

// Force the message loop to serve IDLE routine if there is no other message to process.
class CGLMessageLoop : public CMessageLoop {
  BOOL OnIdle(int nIdleCount) {
    // If the return value is true, continue the idle routine.
    return !CMessageLoop::OnIdle(nIdleCount);
  }
};

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
  CMessageLoop theLoop;
  //CGLMessageLoop theLoop;

	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

  CRect rect(50, 50, 1280 + 50, 768 + 50);

	if(wndMain.CreateEx(NULL, rect) == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
