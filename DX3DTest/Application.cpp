#include "stdafx.h"
#include "Application.h"

Application::Application()
{
}


Application::~Application()
{
}

void Application::Init()
{
	DX::Get()->Init();
	Mouse::Get()->Init(DX::GetInfo().hWnd);
	Camera::Get()->Init();
	SceneManager::Get()->Init();
}

void Application::Destroy()
{
	Mouse::Delete();
	Keyboard::Delete();
	Camera::Delete();
	SceneManager::Delete();
	ObjectManager::Delete();
	g_pMapManager->Destroy();
	FontManager::Delete();
	TextureManager::Delete();
    ImGuiManager::Delete();
	ShaderManager::Delete();
    g_pResourceManager->Destroy();
	Debug::Delete();
	DX::Delete();
}

void Application::Update()
{
    ImGuiManager::Get()->Update();
	//Debug::Get()->InitText();
	Time::Get()->Update();
	Mouse::Get()->Update();
	Keyboard::Get()->Update();
	SceneManager::Get()->Update();
	Camera::Get()->Update();
}

void Application::Render()
{
	DX::GetDevice()->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(64, 64, 64), 1.0f, 0);
	
	DX::GetDevice()->BeginScene();
	
	SceneManager::Get()->Render();

	//Debug::Get()->Print();
    ImGuiManager::Get()->Render();
	DX::GetDevice()->EndScene();
	DX::GetDevice()->Present(NULL, NULL, NULL, NULL);

}

void Application::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Mouse::Get()->InputProc(message, wParam, lParam);
}

