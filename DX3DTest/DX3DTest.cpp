// DX3DTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DX3DTest.h"
#include "Application.h"

//#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console") 


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	D3DInfo info;
	info.appName = _T("DirectX Game");
	info.hInstance = hInstance;
	info.isFullScreen = false;
	info.hWnd = NULL;
	
	if (info.isFullScreen)
	{
		info.screenWidth = GetSystemMetrics(SM_CXSCREEN);
		info.screenHeight = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		info.screenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.9f;
		info.screenHeight = info.screenWidth * 9 / 16.0f;
	}
	DX::SetInfo(info);

	Application* app = new Application;
	app->Run();

	return 0;
}

