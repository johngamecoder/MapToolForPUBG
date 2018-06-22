#include "stdafx.h"
#include "ImGuiManager.h"

ImGuiManager* ImGuiManager::instance = NULL;

ImGuiManager * ImGuiManager::Get()
{
    if (instance == NULL)
        instance = new ImGuiManager();

    return instance;
}

void ImGuiManager::Delete()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    SAFE_DELETE(instance);
}

ImGuiManager::ImGuiManager()
{
}


ImGuiManager::~ImGuiManager()
{
}

void ImGuiManager::Update()
{
    // Start the ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    
    //// 1. Show a simple window.
    //// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
    //{
    //    static float f = 0.0f;
    //    static int counter = 0;
    //    ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
    //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
    //    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
    //    ImGui::Checkbox("Another Window", &show_another_window);
    //    ImGui::Checkbox("testing", &testing);

    //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
    //        counter++;
    //    ImGui::SameLine();
    //    ImGui::Text("counter = %d", counter);

    //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //}

    //// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
    //if (show_another_window)
    //{
    //    ImGui::Begin("Another Window", &show_another_window);
    //    ImGui::Text("Hello from another window!");
    //    if (ImGui::Button("Close Me"))
    //        show_another_window = false;
    //    ImGui::End();
    //}
    //if (testing)
    //{
    //    ImGui::Begin("Another Window2", &show_another_window);
    //    ImGui::Text("Hello from another window!1");
    //    if (ImGui::Button("Close Me"))
    //        testing = false;
    //    ImGui::End();
    //}

    //// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
    //if (show_demo_window)
    //{
    //    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
    //    ImGui::ShowDemoWindow(&show_demo_window);
    //    //ImGui::ShowMetricsWindow();
    //}
}

void ImGuiManager::Render()
{
    // Rendering
    ImGui::EndFrame();
    DX::GetDevice()->SetRenderState(D3DRS_ZENABLE, false);
    DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    DX::GetDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
    //D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*255.0f), (int)(clear_color.y*255.0f), (int)(clear_color.z*255.0f), (int)(clear_color.w*255.0f));
    //DX::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
    //if (DX::GetDevice()->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        //DX::GetDevice()->EndScene();
    }
    //HRESULT result = DX::GetDevice()->Present(NULL, NULL, NULL, NULL);

    //// Handle loss of D3D9 device
    //if (result == D3DERR_DEVICELOST && DX::GetDevice()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
    //{
    //    ImGui_ImplDX9_InvalidateDeviceObjects();
    //    g_pd3dDevice->Reset(&g_d3dpp);
    //    ImGui_ImplDX9_CreateDeviceObjects();
    //}
    //DX::GetDevice()->SetRenderState(D3DRS_ZENABLE, true);
    //DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    //DX::GetDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
}
