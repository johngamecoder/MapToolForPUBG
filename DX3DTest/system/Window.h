#pragma once
class Window
{
public:
	Window();
	virtual ~Window();
	
	void InitWindow();
	WPARAM Run();

	virtual void WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

protected:
	virtual void Init() = 0;
	virtual void Destroy() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

};