#pragma once
#include "./system/Window.h"

class Application : public Window
{
private:
	

public:
	Application();
	~Application();
	
	void WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	void Init();
	void Destroy();
	void Update();
	void Render();
};

