#include "stdafx.h"
#include <stdio.h>
#include <sstream>

#include "ImGuizmoManager.h"
#include "Church.h"
#include "Bandage.h"
#include "BoxCollider.h"

//이것을 header 쪽으로 옴기고 싶은데 어떻게 해야 할까요? ㅠ
const char* ComboObjectList[] = { "Bandage","Church"/*,"Tree","Rock","Ware House"*/ };


ImGuizmoManager::ImGuizmoManager()
{
    m_pCamera = NULL;
    //isRenderCollider = true;
    //for rendering image buttons
    m_pButtonTexture_Handle = NULL; m_pButtonTexture_Handle = TextureManager::Get()->GetTexture(_T("Resource/handle.png"));
    m_pButtonTexture_Translation = NULL; m_pButtonTexture_Translation = TextureManager::Get()->GetTexture(_T("Resource/translation.png"));
    m_pButtonTexture_Rotation = NULL;m_pButtonTexture_Rotation = TextureManager::Get()->GetTexture(_T("Resource/rotation.png"));
    m_pButtonTexture_Scale = NULL; m_pButtonTexture_Scale = TextureManager::Get()->GetTexture(_T("Resource/scale.png"));
    m_pButtonTexture_Bounds = NULL; m_pButtonTexture_Bounds = TextureManager::Get()->GetTexture(_T("Resource/bounds.png"));
    ContainObject(); //loading objects from file using resourcemanager

    //m_pBoxCollider = NULL;
}
ImGuizmoManager::~ImGuizmoManager()
{
    //SAFE_DELETE(m_pCamera); 이건 여기서 중단되는것이 아니다.
    int n = m_vecObjectContainer.size();
    for (int i = 0; i < n; i++)
    {
        SAFE_RELEASE(m_vecObjectContainer[i]);
    }

    for (auto p : m_mapObject)
    {
        for (int i = 0; i < p.second->m_vecBoxCollider.size(); i++)
        {
            SAFE_DELETE(p.second->m_vecBoxCollider[i]);
        }
        SAFE_DELETE(p.second);
    }
        
    m_mapObject.clear();

    SAFE_RELEASE(m_pButtonTexture_Handle);
    SAFE_RELEASE(m_pButtonTexture_Translation);
    SAFE_RELEASE(m_pButtonTexture_Rotation);
    SAFE_RELEASE(m_pButtonTexture_Scale);
    SAFE_RELEASE(m_pButtonTexture_Bounds);

    //SAFE_RELEASE(m_pBoxCollider);
}


void ImGuizmoManager::Init()
{
    hierarchySelectedObjIndex = -1;
    hierarchySelectedColliderIndex = -1;
    m_currentSceneName = "";
    m_mapObject.clear();
    m_mapCount.clear();

    boundSizing = false;

    mCurrentGizmoOperation = ImGuizmo::HANDLE;
    mCurrentGizmoMode = ImGuizmo::WORLD;

    comboSelect = -1;
    
    m_pCurrentObject = NULL;

    m_pCamera = Camera::Get();
    // Camera projection
    isPerspective  = true;
    viewWidth     = 10.f; // for orthographic
    
    //identity matrix
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (i == j)
                objectMatrix[(i * 4) + j] = 1.0f;
            else
                objectMatrix[(i * 4) + j] = 0.0f;
    
    //m_pBoxCollider = new BoxCollider(); m_pBoxCollider->Init();
}

void ImGuizmoManager::Update()
{
    
    MouseHandleMove();

    MenuBarImGui();
    HierarchyImGui();
    LoadObjectImGui();
    InspectorImGui();
    
    if (m_pCurrentObject)
    {
        for (int i = 0; i < m_pCurrentObject->m_vecBoxCollider.size(); i++)
        {
            //m_pCurrentObject->m_vecBoxCollider[i]->Update();
            SAFE_UPDATE(m_pCurrentObject->m_vecBoxCollider[i]);
            //m_pCurrentObject->m_vecBoxCollider[i]->Update();

        }
    }
    

    //여기에 transform 바꾸는거 빼놓자
    //SAFE_UPDATE(m_pBoxCollider);
}

void ImGuizmoManager::Render()
{
    //rendering the objects ex) bandage, church
    for (auto p : m_mapObject)
    {
        auto instance = p.second;
        instance->objPtr->SetWorldMatrix(instance->m_matTransform);
        SAFE_RENDER(instance->objPtr);
        for (int i = 0; i < instance->m_vecBoxCollider.size(); i++)
        {
            SAFE_RENDER(instance->m_vecBoxCollider[i]);
        }
    }
    
    //SAFE_RENDER(m_pBoxCollider);
}



void ImGuizmoManager::MenuBarImGui()
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        //ImGui::MenuItem("(dummy menu)", NULL, false, false);
        if (ImGui::MenuItem("New Scene")) Init();
        //ImGui::OpenPopup("Delete?");
        //if(ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        //{
        //    ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
        //    ImGui::Separator();

        //    //static int dummy_i = 0;
        //    //ImGui::Combo("Combo", &dummy_i, "Delete\0Delete harder\0");

        //    static bool dont_ask_me_next_time = false;
        //    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        //    ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
        //    ImGui::PopStyleVar();

        //    if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        //    ImGui::SetItemDefaultFocus();
        //    ImGui::SameLine();
        //    if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        //    ImGui::EndPopup();
        //}

        if (ImGui::MenuItem("Open Scene", "Ctrl+O")) 
        {
            string fileName = "save.txt";
            OpenScene(fileName);
        }
        ImGui::Separator();
        if (ImGui::BeginMenu("Open Recent"))
        {
            ImGui::MenuItem("fish_hat.c");
            ImGui::MenuItem("fish_hat.inl");
            ImGui::MenuItem("fish_hat.h");
            if (ImGui::BeginMenu("More.."))
            {
                ImGui::MenuItem("Hello");
                ImGui::MenuItem("Sailor");
                if (ImGui::BeginMenu("Recurse.."))
                {
                    //ShowExampleMenuFile();
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Save", "Ctrl+S")) 
        {
            string fileName ="save.txt";
            SaveScene(fileName);
        }
        if (ImGui::MenuItem("Save As..")) 
        {

        }
        ImGui::Separator();
        if (ImGui::BeginMenu("Options"))
        {
            static bool enabled = true;
            ImGui::MenuItem("Enabled", "", &enabled);
            ImGui::BeginChild("child", ImVec2(0, 60), true);
            for (int i = 0; i < 10; i++)
                ImGui::Text("Scrolling Text %d", i);
            ImGui::EndChild();
            static float f = 0.5f;
            static int n = 0;
            static bool b = true;
            ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
            ImGui::InputFloat("Input", &f, 0.1f);
            ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
            ImGui::Checkbox("Check", &b);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Colors"))
        {
            float sz = ImGui::GetTextLineHeight();
            for (int i = 0; i < ImGuiCol_COUNT; i++)
            {
                const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
                ImVec2 p = ImGui::GetCursorScreenPos();
                ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
                ImGui::Dummy(ImVec2(sz, sz));
                ImGui::SameLine();
                ImGui::MenuItem(name);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Disabled", false)) // Disabled
        {
            IM_ASSERT(0);
        }
        if (ImGui::MenuItem("Checked", NULL, true)) {}
        if (ImGui::MenuItem("Quit", "Alt+F4")) {}
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", "CTRL+X")) {}
        if (ImGui::MenuItem("Copy", "CTRL+C")) {}
        if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}

void ImGuizmoManager::HierarchyImGui()
{
    ImGui::SetNextWindowPos(ImVec2(30, 30));
    ImGui::Begin(" ");
    {
        int frame_padding_notSelected = 1;
        int frame_padding_Selected = 3;
        switch (mCurrentGizmoOperation)
        {
        case ImGuizmo::HANDLE:
            if (ImGui::ImageButton((void*)m_pButtonTexture_Handle, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1),    frame_padding_Selected, ImColor(0, 0, 0, 255)))    { mCurrentGizmoOperation = ImGuizmo::HANDLE;    }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Translation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Rotation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE;    }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Scale, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE;     }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Bounds, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::BOUNDS; }   ImGui::SameLine();
            boundSizing = false;
            break;                                                                                                                                                                                                
        case ImGuizmo::TRANSLATE:                                                                                                                                                                                 
            if (ImGui::ImageButton((void*)m_pButtonTexture_Handle, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1),    frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::HANDLE;    }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Translation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_Selected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Rotation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Scale, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Bounds, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::BOUNDS; }   ImGui::SameLine();
            boundSizing = false;
            break;                                                                                                                                                                                                 
        case ImGuizmo::ROTATE:                                                                                                                                                                                 
            if (ImGui::ImageButton((void*)m_pButtonTexture_Handle, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::HANDLE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Translation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Rotation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_Selected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Scale, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Bounds, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::BOUNDS; }   ImGui::SameLine();
            boundSizing = false;
            break;                                                                                                                                                                                               
        case ImGuizmo::SCALE:                                                                                                                                                                               
            if (ImGui::ImageButton((void*)m_pButtonTexture_Handle, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::HANDLE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Translation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Rotation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Scale, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_Selected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Bounds, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::BOUNDS; }   ImGui::SameLine();
            boundSizing = false;
            break;
        case ImGuizmo::BOUNDS:
            if (ImGui::ImageButton((void*)m_pButtonTexture_Handle, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::HANDLE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Translation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Rotation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Scale, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Bounds, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_Selected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::BOUNDS; }   ImGui::SameLine();
            boundSizing = true;
            break;
        }

    }ImGui::End();
    
    ImGui::SetNextWindowPos(ImVec2(30, 110));
    ImGui::Begin("Hierarchy");
    {
        ImGui::Separator();
        //이곳에 load한 product들이 들어갈 것이다
        static int Selected = -1;
        static int selected = -1;
        int n = 0;
        if (hierarchySelectedObjIndex != -1)
            Selected = hierarchySelectedObjIndex;
        for (auto p : m_mapObject)
        {
            char buf[32];
            sprintf_s(buf, p.second->m_ObjName.c_str());
            if (ImGui::Selectable(buf, Selected == n))
            {
                Selected = n;
                hierarchySelectedObjIndex = n;
                SetCurrentObject(p.second);
                hierarchySelectedColliderIndex = -1;
                selected = -1;
            }            
            if (hierarchySelectedObjIndex == n)
            {
                for (int i = 0; i < p.second->m_vecBoxCollider.size(); i++)
                {
                    char buf2[50];
                    sprintf_s(buf2, "   Box Collider %d", i);
                    if (ImGui::Selectable(buf2, selected == i))
                    {
                        selected = i;
                        hierarchySelectedColliderIndex = i;
                        D3DXMATRIXA16 matWorldforBoxCollider = m_pCurrentObject->m_vecBoxCollider[i]->m_mTransform * m_pCurrentObject->m_vecBoxCollider[i]->m_mParentTransform;
                        MatChangeDX2Float(objectMatrix, &matWorldforBoxCollider);
                        //D3DXMATRIXA16 matTransform;
                        //MatChangeFloat2DX(&matTransform, objectMatrix);
                        //MatChangeDX2Float(objectMatrix, &matTransform);
                        //m_pCurrentObject->m_vecBoxCollider[i]->SetMatrix(matTransform, m_pCurrentObject->m_matTransform);
                        //MatChangeDX2Float(objectMatrix, &m_pCurrentObject->m_vecBoxCollider[i]->m_mTransform);
                    }
                }
            }
            n++;
        }

    }ImGui::End();

}
void ImGuizmoManager::LoadObjectImGui()
{
    ImGui::SetNextWindowPos(ImVec2(30, 500));
    ImGui::Begin("Object Loader");
    {

        ImGui::Combo("", &comboSelect, ComboObjectList, ObjList::COUNT/*이건 갯수 넣는 부분 */);
        ImGui::SameLine();
        if (ImGui::Button("Load"))
        {
            ObjectLoader(comboSelect);
            hierarchySelectedColliderIndex = -1;
            hierarchySelectedObjIndex = m_mapObject.size() - 1;
        }

        ImGui::Separator();

        if (hierarchySelectedObjIndex != -1)
        {
            char buf[50];
            string ment = "You have selected : ";
            ment += m_pCurrentObject->m_ObjName.c_str();
            sprintf_s(buf, ment.c_str());
            ImGui::Text(buf);
            if (ImGui::Button("Add Box Collider"))
            {
                AddBoxCollider();
            }
            if (ImGui::Button("Delete Object"))
            {

            }
        }

        //// Simplified one-liner Combo() API, using values packed in a single constant string
        //static int temp = 0;
        //char buf[100];
        //string str;
        //string str2 = "\0";
        //for (auto p : m_mapObject)
        //{
        //    str += p.second->m_ObjName.c_str();
        //    str += str2.c_str();
        //}
        //sprintf_s(buf, str.c_str());
        //ImGui::Combo("combo", &temp, buf);

        //ImGui::Combo("",&temp)




    }ImGui::End();
}
void ImGuizmoManager::InspectorImGui()
{
    ImGui::Begin("Inspector");
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        if (!m_pCurrentObject) //if current object is not picked then 
        {
            ImGui::End();
            return;
        }

        if (!m_pCamera->isPerspective)
        {
            m_pCamera->m_viewHeight = m_pCamera->m_viewWidth * io.DisplaySize.y / io.DisplaySize.x;
        }

        MatChangeDX2Float(cameraView, m_pCamera->GetViewMatrix());
        MatChangeDX2Float(cameraProjection, m_pCamera->GetProjMatrix());
    
        //ImGuizmo::DrawCube(cameraView, cameraProjection, objectMatrix);

        ImGuizmo::BeginFrame();//찾았다! 이거다이거! 


        
        ImGui::Separator();
        {//변환
            ImGui::Text("Transform");
            EditTransform(cameraView, cameraProjection, objectMatrix);

            if (hierarchySelectedColliderIndex != -1)
            {
                //인버스!!
                D3DXMATRIXA16 matLocalTransform, matWorld, matInv;
                MatChangeFloat2DX(&matWorld, objectMatrix);

                D3DXMatrixInverse(&matInv, NULL, &m_pCurrentObject->m_vecBoxCollider[hierarchySelectedColliderIndex]->m_mParentTransform);
                matLocalTransform = matWorld * matInv;
                
                m_pCurrentObject->m_vecBoxCollider[hierarchySelectedColliderIndex]->m_mTransform = matLocalTransform;

                //MatChangeFloat2DX(&m_pCurrentObject->m_vecBoxCollider[hierarchySelectedColliderIndex]->m_mTransform, objectMatrix);
                //MatChangeFloat2DX(&m_pCurrentObject->m_vecBoxCollider[hierarchySelectedColliderIndex]->m_matWorld, objectMatrix);
                //MatChangeDX2Float(objectMatrix, &m_pCurrentObject->m_vecBoxCollider[hierarchySelectedColliderIndex]->m_matWorld);

                //D3DXMATRIXA16 transform;
                //MatChangeFloat2DX(&transform, objectMatrix);
                //m_pCurrentObject->m_vecBoxCollider[hierarchySelectedColliderIndex]->SetMatrix(transform,m_pCurrentObject->m_matTransform);
            }
            else
                MatChangeFloat2DX(&m_pCurrentObject->m_matTransform, objectMatrix);
            //m_pBoxCollider->SetWorldMatrix(m_pCurrentObject->m_matTransform);

        }
        ImGui::Separator();
        {//카메라
            ImGui::Text("Camera");
            if (ImGui::RadioButton("Perspective", m_pCamera->isPerspective)) m_pCamera->isPerspective = true;
            ImGui::SameLine();
            if (ImGui::RadioButton("Orthographic", !m_pCamera->isPerspective)) m_pCamera->isPerspective = false;
            if (m_pCamera->isPerspective)
            {
                ImGui::SliderFloat("Fov", &m_pCamera->m_fov, D3DX_PI / 4, D3DX_PI / 2);
                ImGui::SliderFloat("Distance", &m_pCamera->m_distance, 1.f, 100.f);
            }
            else
            {
                ImGui::SliderFloat("Ortho width", &m_pCamera->m_viewWidth, 1, 20);
            }
            ImGui::SliderAngle("Camera X", &m_pCamera->m_rotY, 0.f, 179.f);
            ImGui::SliderAngle("Camera Y", &m_pCamera->m_rotX, 0.f, 179.f);
            ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);

        }
    }ImGui::End();
}



void ImGuizmoManager::NewScene()
{
    for (auto p : m_mapObject)
    {
        for (int i = 0; i < p.second->m_vecBoxCollider.size(); i++)
        {
            SAFE_DELETE(p.second->m_vecBoxCollider[i]);
        }
        SAFE_DELETE(p.second);
    }       

    Init();
}
void ImGuizmoManager::OpenScene(string& fileName)
{
    Init(); // clearing the scene

    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        string line;
        getline(myFile, line);
        m_currentSceneName = line;  //opening current scene 

        while (!myFile.eof())
        {
            std::stringstream ss;
            std::stringstream ssMat;
            getline(myFile, line); //take out {
            if (line != "{") break;
            ObjInfo* temp = new ObjInfo();
            getline(myFile, line);                      temp->ID = stoi(line);
            getline(myFile, line);                      temp->list = ObjList(stoi(line));
            getline(myFile, line);                      temp->m_ObjName += line;
            getline(myFile, line);      ss << line;     ss >> line;     temp->m_Position.x = stof(line, 0);     ss >> line;     temp->m_Position.y = stof(line, 0);      ss >> line;    temp->m_Position.z = stof(line, 0);
            getline(myFile, line);      ss << line;     ss >> line;     temp->m_Rotation.x = stof(line, 0);     ss >> line;     temp->m_Rotation.y = stof(line, 0);      ss >> line;    temp->m_Rotation.z = stof(line, 0);
            getline(myFile, line);      ss << line;     ss >> line;     temp->m_Scale.x = stof(line, 0);        ss >> line;     temp->m_Scale.y = stof(line, 0);         ss >> line;    temp->m_Scale.z = stof(line, 0);
            
            getline(myFile, line);      ssMat << line;
            ssMat >> line; temp->m_matTransform._11 = stof(line, 0);  ssMat >> line; temp->m_matTransform._12 = stof(line, 0);  ssMat >> line; temp->m_matTransform._13 = stof(line, 0);  ssMat >> line; temp->m_matTransform._14 = stof(line, 0);
            ssMat >> line; temp->m_matTransform._21 = stof(line, 0);  ssMat >> line; temp->m_matTransform._22 = stof(line, 0);  ssMat >> line; temp->m_matTransform._23 = stof(line, 0);  ssMat >> line; temp->m_matTransform._24 = stof(line, 0);
            ssMat >> line; temp->m_matTransform._31 = stof(line, 0);  ssMat >> line; temp->m_matTransform._32 = stof(line, 0);  ssMat >> line; temp->m_matTransform._33 = stof(line, 0);  ssMat >> line; temp->m_matTransform._34 = stof(line, 0);
            ssMat >> line; temp->m_matTransform._41 = stof(line, 0);  ssMat >> line; temp->m_matTransform._42 = stof(line, 0);  ssMat >> line; temp->m_matTransform._43 = stof(line, 0);  ssMat >> line; temp->m_matTransform._44 = stof(line, 0);
            
            getline(myFile, line);
            if (line == "[")
            {
                getline(myFile, line);
                int colliderNum = atoi(line.c_str());
                for (int i = 0; i < colliderNum; i++)
                {
                    getline(myFile, line);  //take out "("
                    getline(myFile, line);  //get parent name;
                    BoxCollider* bc = new BoxCollider(line);
                    bc->Init(D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DXVECTOR3(0.5f, 0.5f, 0.5f));
                    
                    getline(myFile, line);      ss << line;     ss >> line;    bc->m_vCenter.x = stof(line, 0);     ss >> line;     bc->m_vCenter.y = stof(line, 0);      ss >> line;    bc->m_vCenter.z = stof(line, 0);
                    getline(myFile, line);      ss << line;     ss >> line;    bc->m_vExtent.x = stof(line, 0);     ss >> line;     bc->m_vExtent.y = stof(line, 0);      ss >> line;    bc->m_vExtent.z = stof(line, 0);

                    getline(myFile, line);      ssMat << line;
                    ssMat >> line; bc->m_mTransform._11 = stof(line, 0);  ssMat >> line; bc->m_mTransform._12 = stof(line, 0);  ssMat >> line; bc->m_mTransform._13 = stof(line, 0);  ssMat >> line; bc->m_mTransform._14 = stof(line, 0);
                    ssMat >> line; bc->m_mTransform._21 = stof(line, 0);  ssMat >> line; bc->m_mTransform._22 = stof(line, 0);  ssMat >> line; bc->m_mTransform._23 = stof(line, 0);  ssMat >> line; bc->m_mTransform._24 = stof(line, 0);
                    ssMat >> line; bc->m_mTransform._31 = stof(line, 0);  ssMat >> line; bc->m_mTransform._32 = stof(line, 0);  ssMat >> line; bc->m_mTransform._33 = stof(line, 0);  ssMat >> line; bc->m_mTransform._34 = stof(line, 0);
                    ssMat >> line; bc->m_mTransform._41 = stof(line, 0);  ssMat >> line; bc->m_mTransform._42 = stof(line, 0);  ssMat >> line; bc->m_mTransform._43 = stof(line, 0);  ssMat >> line; bc->m_mTransform._44 = stof(line, 0);

                    temp->m_vecBoxCollider.push_back(bc);
                    getline(myFile, line);//take out ")";

                }
                getline(myFile, line); //take out "]"
                getline(myFile, line);  //take out "}"
            }
            else
            {
                getline(myFile, line);  //take out "}"
            }




            temp->objPtr = m_vecObjectContainer[temp->list];

            
            //m_mapObject.emplace(temp->m_ObjName, temp);
            m_mapObject.emplace_back(make_pair(temp->m_ObjName, temp));
        }
        myFile.close();
    }
    else
    {
        assert(false && "안됨.ㅜㅜ");
    }

    for (auto p : m_mapObject)
    {
        if (m_mapCount.find(p.second->list) == m_mapCount.end() )
        {
            m_mapCount.emplace(p.second->list, 0);
        }
        m_mapCount[p.second->list]++;
    }
}

void ImGuizmoManager::SaveScene(string& fileName)
{
    std::ofstream myFile(fileName);
    if (myFile.is_open())
    {
        myFile << fileName << "\n";
        myFile <<"Num of Object :"<<m_mapObject.size()<< "\n";
        
        for (auto p : m_mapObject)
        {
            ObjInfo* temp = p.second;
            myFile << "{"<<"\n";
            myFile << to_string(temp->ID) + "\n";
            myFile << to_string(temp->list) + "\n";
            myFile << temp->m_ObjName + "\n";
            myFile << to_string(temp->m_Position.x) + " " + to_string(temp->m_Position.y) + " " + to_string(temp->m_Position.z) + "\n";
            myFile << to_string(temp->m_Rotation.x) + " " + to_string(temp->m_Rotation.y) + " " + to_string(temp->m_Rotation.z) + "\n";
            myFile << to_string(temp->m_Scale.x) + " " + to_string(temp->m_Scale.y) + " " + to_string(temp->m_Scale.z) + "\n";
            myFile << to_string(temp->m_matTransform._11) + " " + to_string(temp->m_matTransform._12) + " " + to_string(temp->m_matTransform._13) + " " + to_string(temp->m_matTransform._14) + " "
                + to_string(temp->m_matTransform._21) + " "+ to_string(temp->m_matTransform._22) + " "+ to_string(temp->m_matTransform._23) + " "+ to_string(temp->m_matTransform._24) + " "
                + to_string(temp->m_matTransform._31) + " "+ to_string(temp->m_matTransform._32) + " "+ to_string(temp->m_matTransform._33) + " "+ to_string(temp->m_matTransform._34) + " "
                + to_string(temp->m_matTransform._41) + " "+ to_string(temp->m_matTransform._42) + " "+ to_string(temp->m_matTransform._43) + " "+ to_string(temp->m_matTransform._44) + "\n";
            int colliderNum = temp->m_vecBoxCollider.size();
            if(colliderNum > 0)
            {
                myFile << "[" << "\n";
                myFile << colliderNum << "\n";
                for (int i = 0; i < colliderNum; i++)
                {
                    myFile << "(" << "\n";
                    myFile << temp->m_vecBoxCollider[i]->m_Parentname << "\n";
                    myFile << to_string(temp->m_vecBoxCollider[i]->m_vCenter.x) + " " + to_string(temp->m_vecBoxCollider[i]->m_vCenter.y) + " " + to_string(temp->m_vecBoxCollider[i]->m_vCenter.z) + "\n";
                    myFile << to_string(temp->m_vecBoxCollider[i]->m_vExtent.x) + " " + to_string(temp->m_vecBoxCollider[i]->m_vExtent.y) + " " + to_string(temp->m_vecBoxCollider[i]->m_vExtent.z) + "\n";
                    myFile << to_string(temp->m_vecBoxCollider[i]->m_mTransform._11) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._12) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._13) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._14) + " "
                            + to_string(temp->m_vecBoxCollider[i]->m_mTransform._21) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._22) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._23) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._24) + " "
                            + to_string(temp->m_vecBoxCollider[i]->m_mTransform._31) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._32) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._33) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._34) + " "
                            + to_string(temp->m_vecBoxCollider[i]->m_mTransform._41) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._42) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._43) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._44) + "\n";

                    myFile << ")" << "\n";
                }
                myFile << "]" << "\n";
            }
            
            myFile << "}"<<"\n";
        }
        myFile.close();
    }
    else
    {
        assert(false && "안됨..");
    }

}

void ImGuizmoManager::ContainObject()
{
    m_vecObjectContainer.resize(ObjList::COUNT);
    //m_vecObjectContainer.resize(2);

    m_vecObjectContainer[0] = new Bandage(); 
    m_vecObjectContainer[1] = new Church(); 
    
    for (int i = 0; i < m_vecObjectContainer.size(); i++)
    {
        m_vecObjectContainer[i]->Init(); 
        m_vecObjectContainer[i]->name = ComboObjectList[i];
        m_mapCount.emplace((ObjList)i, 0);
    }
    //m_vecObjectContainer[ObjList::TREE] = new TREE();
    //m_vecObjectContainer[ObjList::ROCK] = new ROCK();
    //m_vecObjectContainer[ObjList::WAREHOUSE] = new WAREHOUSE();


    //m_mapCount.emplace((ObjList)1, 0);
    //m_mapCount.emplace(ObjList::TREE, 0);
    //m_mapCount.emplace(ObjList::ROCK, 0);
    //m_mapCount.emplace(ObjList::WAREHOUSE, 0);

}
void ImGuizmoManager::EditTransform(const float * cameraView, float * cameraProjection, float * matrix)
{
    
    //static bool useSnap = false;
    //static float snap[3] = { 1.f, 1.f, 1.f };
    static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
    //static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
    
    //static bool boundSizingSnap = false;
    
    if (ImGui::IsKeyPressed(81))  // q Key
    {
        mCurrentGizmoOperation = ImGuizmo::HANDLE;
        boundSizing = false;
    }
    if (ImGui::IsKeyPressed(87)) // w key
    {
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        boundSizing = false;
    }
    if (ImGui::IsKeyPressed(69))// e key
    {
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
        boundSizing = false;
    }    
    if (ImGui::IsKeyPressed(82)) // r Key
    {
        mCurrentGizmoOperation = ImGuizmo::SCALE;
        boundSizing = false;
    }
    if (ImGui::IsKeyPressed(84)) // t Key
    {
        mCurrentGizmoOperation = ImGuizmo::BOUNDS;
        boundSizing = true;
    }
        

    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
        mCurrentGizmoOperation = ImGuizmo::SCALE;

    float matrixTranslation[3], matrixRotation[3], matrixScale[3];

    ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);//메트릭스에서 각 요소들 구하는 부분
    ImGui::InputFloat3("Tr", matrixTranslation, 3);
    ImGui::InputFloat3("Rt", matrixRotation, 3);
    ImGui::InputFloat3("Sc", matrixScale, 3);
    m_pCurrentObject->m_Position = D3DXVECTOR3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
    m_pCurrentObject->m_Rotation = D3DXVECTOR3(matrixRotation[0], matrixRotation[1], matrixRotation[2]);
    m_pCurrentObject->m_Scale = D3DXVECTOR3(matrixScale[0], matrixScale[1], matrixScale[2]);
    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

    if (mCurrentGizmoOperation != ImGuizmo::SCALE)
    {
        if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
            mCurrentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
            mCurrentGizmoMode = ImGuizmo::WORLD;
    }

    ImGui::Checkbox("Bound Sizing", &boundSizing);

    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL,  NULL, boundSizing ? bounds : NULL, NULL);
    
}
void ImGuizmoManager::ObjectLoader(int index)
{
    ObjInfo* temp = new ObjInfo(++m_mapCount[(ObjList)index], (ObjList)index);
    temp->objPtr = m_vecObjectContainer[(ObjList)index];
    temp->m_ObjName = m_vecObjectContainer[(ObjList)index]->name + to_string(m_mapCount[(ObjList)index]);
    //m_mapObject.emplace(temp->m_ObjName, temp);

    //
    m_mapObject.emplace_back(make_pair(temp->m_ObjName, temp));
    //

    SetCurrentObject(temp); 
}
void ImGuizmoManager::AddBoxCollider()
{
    BoxCollider* obj = new BoxCollider(m_pCurrentObject->m_ObjName);
    obj->Init(D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DXVECTOR3(0.5f, 0.5f, 0.5f));
    D3DXMATRIXA16 matIdentity;
    D3DXMatrixIdentity(&matIdentity);
    obj->SetMatrix(matIdentity, m_pCurrentObject->m_matTransform);
    m_pCurrentObject->m_vecBoxCollider.push_back(obj);
}

void ImGuizmoManager::DeleteObject()
{
}

void ImGuizmoManager::MouseHandleMove()
{
    //this functions are controlled at Camera.cpp
    if (mCurrentGizmoOperation != ImGuizmo::HANDLE)
        m_pCamera->isHandle = false;
    else
        m_pCamera->isHandle = true;
}

void ImGuizmoManager::MatChangeDX2Float(OUT float * m16, IN D3DXMATRIXA16 * mat)
{
    m16[0] = mat->_11;m16[1] = mat->_12;m16[2] = mat->_13;m16[3] = mat->_14;
    m16[4] = mat->_21;m16[5] = mat->_22;m16[6] = mat->_23;m16[7] = mat->_24;
    m16[8] = mat->_31;m16[9] = mat->_32;m16[10] = mat->_33;m16[11] = mat->_34;
    m16[12] = mat->_41;m16[13] = mat->_42;m16[14] = mat->_43;m16[15] = mat->_44;
}
void ImGuizmoManager::MatChangeFloat2DX(OUT D3DXMATRIXA16 * mat, IN float * m16)
{
    mat->_11 = m16[0];mat->_12 = m16[1];mat->_13 = m16[2]; mat->_14 = m16[3];
    mat->_21 = m16[4];mat->_22 = m16[5];mat->_23 = m16[6]; mat->_24 = m16[7];
    mat->_31 = m16[8];mat->_32 = m16[9];mat->_33 = m16[10];mat->_34 = m16[11];
    mat->_41 = m16[12];mat->_42 = m16[13];mat->_43 = m16[14];mat->_44 = m16[15];
}
