#include "stdafx.h"
#include <stdio.h>
#include <sstream>

#include "ImGuizmoManager.h"
#include "Church.h"
#include "Bandage.h"
#include "BoxCollider.h"

//이것을 header 쪽으로 옴기고 싶은데 어떻게 해야 할까요? ㅠ
const char* ComboObjectList[] = { "Bandage","Church","Tree","Rock","Ware House" };


ImGuizmoManager::ImGuizmoManager()
{
    m_pCamera = NULL;
    
    //for rendering image buttons
    m_pButtonTexture_Handle = NULL; m_pButtonTexture_Handle = TextureManager::Get()->GetTexture(_T("Resource/handle.png"));
    m_pButtonTexture_Translation = NULL; m_pButtonTexture_Translation = TextureManager::Get()->GetTexture(_T("Resource/translation.png"));
    m_pButtonTexture_Rotation = NULL;m_pButtonTexture_Rotation = TextureManager::Get()->GetTexture(_T("Resource/rotation.png"));
    m_pButtonTexture_Scale = NULL; m_pButtonTexture_Scale = TextureManager::Get()->GetTexture(_T("Resource/scale.png"));

    ContainObject(); //loading objects from file using resourcemanager

    m_pBoxCollider = NULL;
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
        SAFE_DELETE(p.second);
    m_mapObject.clear();

    SAFE_RELEASE(m_pButtonTexture_Handle);
    SAFE_RELEASE(m_pButtonTexture_Translation);
    SAFE_RELEASE(m_pButtonTexture_Rotation);
    SAFE_RELEASE(m_pButtonTexture_Scale);

    SAFE_RELEASE(m_pBoxCollider);
}


void ImGuizmoManager::Init()
{
    m_currentSceneName = "";
    m_mapObject.clear();
    bandageCount = 0;
    churchCount = 0;
    treeCount = 0;
    rockCount = 0;
    wareHouseCount = 0;



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
    
    m_pBoxCollider = new BoxCollider(); m_pBoxCollider->Init();
}

void ImGuizmoManager::Update()
{
    MouseHandleMove();
    MenuBarImGui();
    HierarchyImGui();
    LoadObjectImGui();
    InspectorImGui();
    
    //여기에 transform 바꾸는거 빼놓자
    SAFE_UPDATE(m_pBoxCollider);
}

void ImGuizmoManager::Render()
{
    //rendering the objects ex) bandage, church
    for (auto p : m_mapObject)
    {
        auto instance = p.second;
        instance->objPtr->SetWorldMatrix(instance->m_matTransform);
        SAFE_RENDER(instance->objPtr);
    }
    
    SAFE_RENDER(m_pBoxCollider);
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
            break;                                                                                                                                                                                                
        case ImGuizmo::TRANSLATE:                                                                                                                                                                                 
            if (ImGui::ImageButton((void*)m_pButtonTexture_Handle, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1),    frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::HANDLE;    }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Translation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_Selected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Rotation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Scale, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE; }   ImGui::SameLine();
            break;                                                                                                                                                                                                 
        case ImGuizmo::ROTATE:                                                                                                                                                                                 
            if (ImGui::ImageButton((void*)m_pButtonTexture_Handle, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::HANDLE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Translation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Rotation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_Selected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Scale, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE; }   ImGui::SameLine();
            break;                                                                                                                                                                                               
        case ImGuizmo::SCALE:                                                                                                                                                                               
            if (ImGui::ImageButton((void*)m_pButtonTexture_Handle, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::HANDLE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Translation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Rotation, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture_Scale, ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_Selected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE; }   ImGui::SameLine();
            break;
        }

    }ImGui::End();
    
    ImGui::Begin("Hierarchy");
    {
        ImGui::Separator();
        //이곳에 load한 product들이 들어갈 것이다
        static int selected = -1;
        int n = 0;
        for (auto p : m_mapObject)
        {
            char buf[32];
            sprintf_s(buf, p.second->m_ObjName.c_str());
            if (ImGui::Selectable(buf, selected == n))
            {
                selected = n;
                SetCurrentObject(p.second); 
            }
            n++;
        }

    }ImGui::End();
}
void ImGuizmoManager::LoadObjectImGui()
{
    ImGui::Begin("Object Loader");
    {

        ImGui::Combo("", &comboSelect, ComboObjectList, ObjList::COUNT/*이건 갯수 넣는 부분 */);
        ImGui::SameLine();
        if (ImGui::Button("Load"))
        {
            ObjectLoader(comboSelect);
        }

        ImGui::Separator();
        //int temp;
        // Simplified one-liner Combo() API, using values packed in a single constant string
        //static int item_current_2 = 0;
        //ImGui::Combo("combo 2 (one-liner)", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

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
        SAFE_DELETE(p.second);

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
            
            temp->objPtr = m_vecObjectContainer[temp->list];
            getline(myFile, line);  //take out }
            
            m_mapObject.emplace(temp->m_ObjName, temp);
        }
        myFile.close();
    }
    else
    {
        assert(false && "안됨.ㅜㅜ");
    }

    for (auto p : m_mapObject)
    {
        switch (p.second->list)
        {
        case ObjList::BANDAGE:
            bandageCount = p.second->ID;
            break;
        case ObjList::CHURCH:
            churchCount = p.second->ID;
            break;
        case ObjList::TREE:
            treeCount = p.second->ID;
            break;
        case ObjList::ROCK:
            rockCount = p.second->ID;
            break;
        case ObjList::WAREHOUSE:
            wareHouseCount = p.second->ID;
            break;
        }
    }
}

void ImGuizmoManager::SaveScene(string& fileName)
{
    std::ofstream myFile(fileName);
    if (myFile.is_open())
    {
        myFile << fileName << "\n";
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
    //m_vecObjectContainer.resize(ObjList::COUNT);
    m_vecObjectContainer.resize(1);

    m_vecObjectContainer[ObjList::BANDAGE] = new Bandage(); m_vecObjectContainer[ObjList::BANDAGE]->Init();
    //m_vecObjectContainer[ObjList::CHURCH] = new Church(); m_vecObjectContainer[ObjList::CHURCH]->Init();
    //m_vecObjectContainer[ObjList::TREE] = new TREE();
    //m_vecObjectContainer[ObjList::ROCK] = new ROCK();
    //m_vecObjectContainer[ObjList::WAREHOUSE] = new WAREHOUSE();

}
void ImGuizmoManager::EditTransform(const float * cameraView, float * cameraProjection, float * matrix)
{
    
    //static bool useSnap = false;
    //static float snap[3] = { 1.f, 1.f, 1.f };
    static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
    //static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
    static bool boundSizing = false;
    //static bool boundSizingSnap = false;

    if (ImGui::IsKeyPressed(81))  // q Key
        mCurrentGizmoOperation = ImGuizmo::HANDLE;
    if (ImGui::IsKeyPressed(87)) // w key
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(69))// e key
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(82)) // r Key
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    if (ImGui::IsKeyPressed(84))
        mCurrentGizmoOperation = ImGuizmo::BOUNDS;

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
    switch (index)
    {
    case ObjList::BANDAGE:
        SetCurrentObject(new ObjInfo(++bandageCount, ObjList::BANDAGE));
        m_pCurrentObject->objPtr = m_vecObjectContainer[ObjList::BANDAGE];
        m_pCurrentObject->m_ObjName = "Bandage" + to_string(bandageCount);
        m_mapObject.emplace(m_pCurrentObject->m_ObjName, m_pCurrentObject);
        break;
    case ObjList::CHURCH:
        SetCurrentObject(new ObjInfo(++churchCount, ObjList::CHURCH));
        m_pCurrentObject->objPtr = m_vecObjectContainer[ObjList::CHURCH];
        m_pCurrentObject->m_ObjName = "Church" + to_string(churchCount);
        m_mapObject.emplace(m_pCurrentObject->m_ObjName, m_pCurrentObject);
        break;
    case ObjList::TREE:
        
        //m_pCurrentObject = new ObjInfo(++treeCount, ObjList::TREE);
        //m_pCurrentObject->objPtr = m_vecObjectContainer[ObjList::TREE];
        //m_mapObject.emplace("Tree" + to_string(treeCount), m_pCurrentObject);
        //break;
    case ObjList::ROCK:
        //m_pCurrentObject = new ObjInfo(++rockCount, ObjList::ROCK);
        //m_pCurrentObject->objPtr = m_vecObjectContainer[ObjList::ROCK];
        //m_mapObject.emplace("Rock" + to_string(rockCount), m_pCurrentObject);
        //break;
    case ObjList::WAREHOUSE:
        //m_pCurrentObject = new ObjInfo(++wareHouseCount, ObjList::WAREHOUSE);
        //m_pCurrentObject->objPtr = m_vecObjectContainer[ObjList::WAREHOUSE];
        //m_mapObject.emplace("WareHouse" + to_string(wareHouseCount), m_pCurrentObject);
        //break;
        ImGui::Begin("not yet");
        ImGui::Text("load할 것이 없어서 ");
        ImGui::End();
        return;
    }
    
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
