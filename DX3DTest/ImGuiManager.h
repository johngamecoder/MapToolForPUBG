#pragma once
class ImGuiManager
{
public:
    static ImGuiManager*	Get();
    static void		Delete();
private:
    static ImGuiManager*	instance;

    //map<CString, LPDIRECT3DTEXTURE9>	m_mapTexture;
    //map<CString, D3DXIMAGE_INFO>		m_mapImageInfo;

    ImGuiManager();
    ~ImGuiManager();

    bool show_demo_window = true;
    bool show_another_window = false;
    bool testing = false;
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
public:
    void Update();
    void Render();
private:
    //LPDIRECT3DTEXTURE9 _GetTexture(LPCTSTR filePath, OUT D3DXIMAGE_INFO* pImageInfo = NULL);
public:
    //LPDIRECT3DTEXTURE9 GetTexture(LPCTSTR filePath, OUT D3DXIMAGE_INFO* pImageInfo = NULL);
};

