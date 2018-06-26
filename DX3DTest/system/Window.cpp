#include "stdafx.h"
#include "Window.h"

namespace
{
	Window* g_pApp = NULL;
}
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( g_pApp ) g_pApp->WindowProc(hWnd, message, wParam, lParam);
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;
    switch (message)
    {
    case WM_SIZE:
        if (DX::GetDevice() != NULL && wParam != SIZE_MINIMIZED)
        {
            ImGui_ImplDX9_InvalidateDeviceObjects();
            ImGui_ImplDX9_CreateDeviceObjects();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    }
	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

Window::Window()
{
	InitWindow();
}

Window::~Window()
{
	D3DInfo info = DX::GetInfo();
	
	if (info.isFullScreen == true)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(DX::GetInfo().hWnd);

	UnregisterClass(info.appName, info.hInstance);
}

void Window::InitWindow()
{
	D3DInfo info = DX::GetInfo();

	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = info.hInstance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = info.appName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);
    
	WORD wHr = RegisterClassEx(&wndClass);
	assert(wHr != 0);
	
	if (info.isFullScreen == true)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = info.screenWidth;
		devMode.dmPelsHeight = info.screenHeight;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}

	info.hWnd = CreateWindowEx(WS_EX_APPWINDOW, info.appName, info.appName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, info.hInstance, NULL);

	assert(info.hWnd != NULL);
	DX::SetInfo(info);


	RECT rect = { 0, 0, (LONG)info.screenWidth, (LONG)info.screenHeight };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - info.screenWidth) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - info.screenHeight) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow
	(
		info.hWnd
		, centerX, centerY
		, rect.right - rect.left, rect.bottom - rect.top
		, TRUE
	);
	ShowWindow(info.hWnd, SW_SHOWNORMAL);
	SetForegroundWindow(info.hWnd);
	SetFocus(info.hWnd);

	ShowCursor(true);

	g_pApp = this;
}

WPARAM Window::Run()
{
	MSG msg = { 0 };

	Init();
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
			Render();
		}
	}

	Destroy();

	return msg.wParam;
}