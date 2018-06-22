#include "stdafx.h"
#include <stdio.h>
#include "ImGuizmo.h"
#include "ImGuizmoManager.h"
#include "Church.h"

//이것을 header 쪽으로 옴기고 싶은데 어떻게 해야 할까요? ㅠ
const char* ComboObjectList[] = { "Church","Tree","Rock","Ware House" };


ImGuizmoManager::ImGuizmoManager()
{
    comboSelect = 0;
    m_pCamera = NULL;
    m_pCurrentObject = NULL;


    //identity matrix
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (i == j)
                objectMatrix[(i * 4) + j] = 1.0f;
            else
                objectMatrix[(i * 4) + j] = 0.0f;
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
    
}


void ImGuizmoManager::Init()
{
    //g_pResourceManager->GetEffectMesh("h", "h");
    m_pCamera = Camera::Get();
    // Camera projection
    isPerspective  = true;
    viewWidth     = 10.f; // for orthographic

    ContainObject();
}

void ImGuizmoManager::Update()
{
    
    HierarchyImGui();
    LoadObjectImGui();
    InspectorImGui();
    //여기에 transform 바꾸는거 빼놓자
}

void ImGuizmoManager::Render()
{
    for (auto p : m_mapObject)
    {
        auto instance = p.second;
        instance->objPtr->SetWorldMatrix(instance->m_matTransform);
        SAFE_RENDER(instance->objPtr);
    }
    
}



void ImGuizmoManager::HierarchyImGui()
{
    //ImGui::Begin(" ");
    //{
    //    if (ImGui::ArrowButton("Hand", ImGuiDir_::ImGuiDir_Left))
    //        Camera::Get()->m_bQKey = !Camera::Get()->m_bQKey;
    //    ImGui::SameLine();
    //    if (ImGui::ArrowButton("Translation", ImGuiDir_::ImGuiDir_Up))
    //        Camera::Get()->m_bWKey = !Camera::Get()->m_bWKey;
    //    ImGui::SameLine();
    //    ImGui::ArrowButton("Rotation", ImGuiDir_::ImGuiDir_Right);
    //    ImGui::SameLine();
    //    ImGui::ArrowButton("Scale", ImGuiDir_::ImGuiDir_Down);
    //    ImGui::SameLine();
    //}ImGui::End();
    
    ImGui::Begin("Hierarchy");
    {
        ImGui::Separator();
        //이곳에 load한 product들이 들어갈 것이다
        static bool selectedbool = false;
        for (auto p : m_mapObject)
        {
            char buf[32];
            sprintf_s(buf, p.second->m_ObjName.c_str());

            if (ImGui::Selectable(buf, selectedbool))
            {
                SetCurrentObject(p.second);
                //m_pCurrentObject = ;
                //MatChangeDX2Float(objectMatrix, &m_pCurrentObject->m_matTransform);
                break;
            }
        }
    }ImGui::End();
}
void ImGuizmoManager::LoadObjectImGui()
{
    ImGui::Begin("Object Loader");
    {
        ImGui::Separator();

        ImGui::Combo("", &comboSelect, ComboObjectList, ObjList::COUNT/*이건 갯수 넣는 부분 */);
        ImGui::SameLine();
        if (ImGui::Button("Load"))
        {
            ObjectLoader(comboSelect);
        }
        ImGui::Separator();
        if (ImGui::Button("Save to txt file"))
        {
            SaveObjInfo2File();
        }

    }ImGui::End();
}
void ImGuizmoManager::InspectorImGui()
{
    ImGui::Begin("Inspector");
    {
        if (!m_pCurrentObject) //if current object is not picked then 
        {
            ImGui::End();
            return;
        }
        ImGuiIO& io = ImGui::GetIO();

    
    
        if (!m_pCamera->isPerspective)
        {
            m_pCamera->m_viewHeight = m_pCamera->m_viewWidth * io.DisplaySize.y / io.DisplaySize.x;
        }

        MatChangeDX2Float(cameraView, m_pCamera->GetViewMatrix());
        MatChangeDX2Float(cameraProjection, m_pCamera->GetProjMatrix());
    


        ImGuizmo::BeginFrame();//찾았다! 이거다이거! 


        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        ImGui::Separator();
        {//변환
            ImGui::Text("Transform");
            EditTransform(cameraView, cameraProjection, objectMatrix);
            MatChangeFloat2DX(&m_pCurrentObject->m_matTransform, objectMatrix);
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

void ImGuizmoManager::ContainObject()
{
    //m_vecObjectContainer.resize(ObjList::COUNT);
    m_vecObjectContainer.resize(1);

    m_vecObjectContainer[ObjList::CHURCH] = new Church(); m_vecObjectContainer[ObjList::CHURCH]->Init();
    //m_vecObjectContainer[ObjList::TREE] = new TREE();
    //m_vecObjectContainer[ObjList::ROCK] = new ROCK();
    //m_vecObjectContainer[ObjList::WAREHOUSE] = new WAREHOUSE();

}

void ImGuizmoManager::EditTransform(const float * cameraView, float * cameraProjection, float * matrix)
{
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
    //static bool useSnap = false;
    //static float snap[3] = { 1.f, 1.f, 1.f };
    //static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
    //static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
    //static bool boundSizing = false;
    //static bool boundSizingSnap = false;

    if (ImGui::IsKeyPressed(87)) // w key
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(69))// e key
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(82)) // r Key
        mCurrentGizmoOperation = ImGuizmo::SCALE;
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
    //if (ImGui::IsKeyPressed(83))
    //    useSnap = !useSnap;
    //ImGui::Checkbox("", &useSnap);
    //ImGui::SameLine();

    //switch (mCurrentGizmoOperation)
    //{
    //case ImGuizmo::TRANSLATE:
    //    ImGui::InputFloat3("Snap", &snap[0]);
    //    break;
    //case ImGuizmo::ROTATE:
    //    ImGui::InputFloat("Angle Snap", &snap[0]);
    //    break;
    //case ImGuizmo::SCALE:
    //    ImGui::InputFloat("Scale Snap", &snap[0]);
    //    break;
    //}
    //ImGui::Checkbox("Bound Sizing", &boundSizing);
    //if (boundSizing)
    //{
    //    ImGui::PushID(3);
    //    ImGui::Checkbox("", &boundSizingSnap);
    //    ImGui::SameLine();
    //    ImGui::InputFloat3("Snap", boundsSnap);
    //    ImGui::PopID();
    //}

    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    //ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
    ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL,  NULL, NULL, NULL);
}
void ImGuizmoManager::ObjectLoader(int index)
{
    switch (index)
    {
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
void ImGuizmoManager::SaveObjInfo2File()
{
    std::ofstream myFile("save.txt");
    if (myFile.is_open())
    {
        for (auto p : m_mapObject)
        {
            ObjInfo* temp = p.second;
            myFile << "{\n";
            myFile << to_string(temp->ID) + "\n";
            myFile << to_string(temp->list) + "\n";
            myFile << temp->m_ObjName + "\n";
            myFile << to_string(temp->m_Position.x) + to_string(temp->m_Position.y) + to_string(temp->m_Position.z) + "\n";
            myFile << to_string(temp->m_Rotation.x) + to_string(temp->m_Rotation.y) + to_string(temp->m_Rotation.z) + "\n";
            myFile << to_string(temp->m_Scale.x) + to_string(temp->m_Scale.y) + to_string(temp->m_Scale.z) + "\n";
            myFile << to_string(temp->m_matTransform._11) + to_string(temp->m_matTransform._12) + to_string(temp->m_matTransform._13) + to_string(temp->m_matTransform._14)
                + to_string(temp->m_matTransform._21) + to_string(temp->m_matTransform._22) + to_string(temp->m_matTransform._23) + to_string(temp->m_matTransform._24)
                + to_string(temp->m_matTransform._31) + to_string(temp->m_matTransform._32) + to_string(temp->m_matTransform._33) + to_string(temp->m_matTransform._34)
                + to_string(temp->m_matTransform._41) + to_string(temp->m_matTransform._42) + to_string(temp->m_matTransform._43) + to_string(temp->m_matTransform._44) + "\n";
            myFile << "}\n";
        }
        
        myFile.close();
    }
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
