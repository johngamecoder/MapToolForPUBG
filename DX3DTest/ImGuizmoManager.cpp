#include "stdafx.h"
#include "ImGuizmoManager.h"

ImGuizmoManager::ImGuizmoManager()
    : m_currentObjPos(D3DXVECTOR3(0.0f,0.0f,0.0f))
    , m_pCamera(nullptr)
{
   //for rendering image buttons
   m_pButtonTexture[0] = NULL; m_pButtonTexture[0] = TextureManager::Get()->GetTexture(_T("Resource/handle.png"));
   m_pButtonTexture[1] = NULL; m_pButtonTexture[1] = TextureManager::Get()->GetTexture(_T("Resource/translation.png"));
   m_pButtonTexture[2] = NULL; m_pButtonTexture[2] = TextureManager::Get()->GetTexture(_T("Resource/rotation.png"));
   m_pButtonTexture[3] = NULL; m_pButtonTexture[3] = TextureManager::Get()->GetTexture(_T("Resource/scale.png"));
   m_pButtonTexture[4] = NULL; m_pButtonTexture[4] = TextureManager::Get()->GetTexture(_T("Resource/bounds.png"));
    
   ConstructComboObjectList();
   ContainObject(); //loading objects from file using resourcemanager
}
ImGuizmoManager::~ImGuizmoManager()
{
    //SAFE_DELETE(m_pCamera); 이건 여기서 중단되는것이 아니다.
    for (int i = 0; i < static_cast<int>(TAG_RES_STATIC::COUNT); i++)
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
    m_mapObjCount.clear();

    for (int i = 0; i < 5; i++)
    {
        SAFE_RELEASE(m_pButtonTexture[i]);
    }
    //SAFE_RELEASE(m_pBoxCollider);
    for (int i = 0; i < static_cast<int>(TAG_RES_STATIC::COUNT); ++i)
    {
        SAFE_DELETE(ComboObjectList[i]);
         //= new char[256];
    }
}


void ImGuizmoManager::Init()
{
    hierarchySelectedObjIndex = -1;
    hierarchySelectedColliderIndex = -1;
    m_currentSceneName = "";
    
    for (auto p : m_mapObject)
    {
        for (int i = 0; i < p.second->m_vecBoxCollider.size(); i++)
        {
            SAFE_DELETE(p.second->m_vecBoxCollider[i]);
        }
        SAFE_DELETE(p.second);
    }
    m_mapObject.clear();


    boundSizing = false;

    mCurrentGizmoOperation = ImGuizmo::NOTSELECTED;
    mCurrentGizmoMode = ImGuizmo::WORLD;

    comboTerrainFeatureSelect = -1;
    comboItemSelect = -1;
    
    m_pCurrentObject = NULL;

    m_pCamera = Camera::Get();
    m_pCamera->SetGizmoOperation(&mCurrentGizmoOperation);
    m_currentObjPos;
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
            SAFE_UPDATE(m_pCurrentObject->m_vecBoxCollider[i]);
        }
    }

}

void ImGuizmoManager::Render()
{
    //rendering the objects ex) bandage, church
    for (auto p : m_mapObject)
    {
        auto instance = p.second;
        instance->objPtr->SetWorldMatrix(instance->m_matTransform);
        SAFE_RENDER(instance->objPtr);//rendering object
        for (int i = 0; i < instance->m_vecBoxCollider.size(); i++)
        {
            SAFE_RENDER(instance->m_vecBoxCollider[i]); //rendering box collider if has
        }
    }

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
            if (ImGui::ImageButton((void*)m_pButtonTexture[0], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_Selected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::HANDLE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[1], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[2], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[3], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[4], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::BOUNDS; }   ImGui::SameLine();
            boundSizing = false;
            break;                                                                                                                                                                                                
        case ImGuizmo::TRANSLATE:                                                                                                                                                                                 
            if (ImGui::ImageButton((void*)m_pButtonTexture[0], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::HANDLE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[1], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_Selected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[2], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[3], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[4], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::BOUNDS; }   ImGui::SameLine();
            boundSizing = false;
            break;                                                                                                                                                                                                 
        case ImGuizmo::ROTATE:                                                                                                                                                                                 
            if (ImGui::ImageButton((void*)m_pButtonTexture[0], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::HANDLE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[1], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[2], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_Selected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[3], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[4], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::BOUNDS; }   ImGui::SameLine();
            boundSizing = false;
            break;                                                                                                                                                                                               
        case ImGuizmo::SCALE:                                                                                                                                                                               
            if (ImGui::ImageButton((void*)m_pButtonTexture[0], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::HANDLE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[1], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[2], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[3], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_Selected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[4], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::BOUNDS; }   ImGui::SameLine();
            boundSizing = false;
            break;
        case ImGuizmo::BOUNDS:
            if (ImGui::ImageButton((void*)m_pButtonTexture[0], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::HANDLE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[1], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[2], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[3], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[4], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_Selected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::BOUNDS; }   ImGui::SameLine();
            boundSizing = true;
            break;
        case ImGuizmo::NOTSELECTED:
            if (ImGui::ImageButton((void*)m_pButtonTexture[0], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::HANDLE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[1], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::TRANSLATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[2], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::ROTATE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[3], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::SCALE; }   ImGui::SameLine();
            if (ImGui::ImageButton((void*)m_pButtonTexture[4], ImVec2(32, 22), ImVec2(0, 0), ImVec2(1, 1), frame_padding_notSelected, ImColor(0, 0, 0, 255))) { mCurrentGizmoOperation = ImGuizmo::BOUNDS; }   ImGui::SameLine();
            boundSizing = false;
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
                        SendCurrentObjBoxColliderPos(D3DXVECTOR3(matWorldforBoxCollider._41, matWorldforBoxCollider._42, matWorldforBoxCollider._43));
                        MatChangeDX2Float(objectMatrix, &matWorldforBoxCollider);
                    }
                }
            }
            n++;
        }

    }ImGui::End();

}

void ImGuizmoManager::InspectorImGui()
{
    ImGui::Begin("Inspector");
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        if (Keyboard::Get()->KeyUp('Q'))  // q Key
        {
            if(mCurrentGizmoOperation == ImGuizmo::HANDLE)
                mCurrentGizmoOperation = ImGuizmo::NOTSELECTED;
            else
                mCurrentGizmoOperation = ImGuizmo::HANDLE;
            boundSizing = false;
        }
        if (m_pCurrentObject) //if current object is not picked then 
        {

        

            if (!m_pCamera->isPerspective)
            {
                m_pCamera->m_viewHeight = m_pCamera->m_viewWidth * io.DisplaySize.y / io.DisplaySize.x;
            }

            MatChangeDX2Float(cameraView, m_pCamera->GetViewMatrix());
            MatChangeDX2Float(cameraProjection, m_pCamera->GetProjMatrix());
    
       

            ImGuizmo::BeginFrame(); 


        
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
        }
    }ImGui::End();
}


void ImGuizmoManager::EditTransform(const float * cameraView, float * cameraProjection, float * matrix)
{
    
    //static bool useSnap = false;
    //static float snap[3] = { 1.f, 1.f, 1.f };
    static float bounds[] = { -5.0f, -5.0f, -5.0f, 5.0f, 5.0f, 5.0f };
    //static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
    
    //static bool boundSizingSnap = false;
    

    if (Keyboard::Get()->KeyUp('W')) // w key
    {
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        boundSizing = false;
    }
    if (Keyboard::Get()->KeyUp('E'))// e key
    {
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
        boundSizing = false;
    }    
    if (Keyboard::Get()->KeyUp('R')) // r Key
    {
        mCurrentGizmoOperation = ImGuizmo::SCALE;
        boundSizing = false;
    }
    if (Keyboard::Get()->KeyUp('T')) // t Key
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
    if (hierarchySelectedObjIndex != -1 && hierarchySelectedColliderIndex == -1)
    {
        m_pCurrentObject->m_Position = D3DXVECTOR3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
        m_pCurrentObject->m_Rotation = D3DXVECTOR3(matrixRotation[0], matrixRotation[1], matrixRotation[2]);
        m_pCurrentObject->m_Scale = D3DXVECTOR3(matrixScale[0], matrixScale[1], matrixScale[2]);
    }
    //if (hierarchySelectedObjIndex != -1 && hierarchySelectedColliderIndex != -1)
    //{
    //    m_pCurrentObject->m_vecBoxCollider[hierarchySelectedColliderIndex]->m_Position = D3DXVECTOR3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
    //    m_pCurrentObject->m_vecBoxCollider[hierarchySelectedColliderIndex]->m_Rotation = D3DXVECTOR3(matrixRotation[0], matrixRotation[1], matrixRotation[2]);
    //    m_pCurrentObject->m_vecBoxCollider[hierarchySelectedColliderIndex]->m_Scale = D3DXVECTOR3(matrixScale[0], matrixScale[1], matrixScale[2]);
    //}

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
void ImGuizmoManager::AddBoxCollider()
{
    BoxCollider* bc = new BoxCollider(&m_pCurrentObject->m_ObjName);
    bc->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), D3DXVECTOR3(5.0f, 5.0f, 5.0f));
    D3DXMATRIXA16 matIdentity;
    D3DXMatrixIdentity(&matIdentity);
    bc->SetMatrix(matIdentity, m_pCurrentObject->m_matTransform);
    m_pCurrentObject->m_vecBoxCollider.push_back(bc);
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

void ImGuizmoManager::ObjectLoaderButton(char* ComboList[], int selectedCombo)
{
    static int comboNumForTF = -1;


    static char buf1[64];
    static bool bLoadButton = false;
    static bool bNotFoundLoad = false;
    if (ImGui::Button("Load"))
    {
        //선택된 번호를 right list로 변환시켜주자.
        for (int i = 0; i < static_cast<int>(TAG_RES_STATIC::COUNT); i++)
        {
            if (ComboList[selectedCombo]==ComboObjectList[i])
            {
                comboNumForTF = i;
                break;
            }
        }
        
        if (!m_vecObjectContainer[comboNumForTF])
        {
            //bNotFoundLoad = true;
            
        }
        else
        {
            string a = " (";
            string b = ")";
            string userInputName = ComboObjectList[comboNumForTF] + a + to_string(m_mapObjCount[static_cast<TAG_RES_STATIC>(comboNumForTF)] + 1) + b;
            sprintf_s(buf1, userInputName.c_str());
            bLoadButton = true;
        }

    }
    if (bLoadButton)
    {
        ImGui::OpenPopup("Please name your Object");
        if (ImGui::BeginPopupModal("Please name your Object", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {

            ImGui::InputText("", buf1, 64);
            static bool bAlreadyHaveName = false;
            if (bAlreadyHaveName)
                ImGui::Text("Already Have that Name, Please Enter new Name");
            ImGui::Separator();
            if (ImGui::Button("OK", ImVec2(120, 0))) {

                if (m_mapObject.find(buf1) == m_mapObject.end())
                {
                    ObjectLoader(comboNumForTF, buf1);
                    hierarchySelectedColliderIndex = -1;
                    hierarchySelectedObjIndex = m_mapObject.size() - 1;
                    bLoadButton = false;
                    bAlreadyHaveName = false;

                    //혹시 몰라서 마지막 백업을 해놓았다
                    string backup = "./Resource/SaveBackUp/UpdateBackup/updateSave.txt";
                    SaveScene(backup);
                }
                else
                {
                    bAlreadyHaveName = true;
                }
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                bLoadButton = false;
                bAlreadyHaveName = false;
            }

        }
        ImGui::EndPopup();
    }

    //if (bNotFoundLoad)
    //{
    //    ImGui::OpenPopup("Object Not Found");
    //    if (ImGui::BeginPopupModal("Sorry, Cannot found Object", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    //    {
    //        if (ImGui::Button("Cancel", ImVec2(120, 0)))
    //        {
    //            bNotFoundLoad = false;
    //        }
    //    }
    //    ImGui::EndPopup();
    //}

}

void ImGuizmoManager::ObjectItemLoaderButton(char * ComboList[], int selectedCombo)
{
    static int comboNumForItm = -1;


    static char buf1[64];
    static bool bLoadButton = false;
    static bool bNotFoundLoad = false;
    if (ImGui::Button("Load"))
    {
        //선택된 번호를 right list로 변환시켜주자.
        for (int i = 0; i < static_cast<int>(TAG_RES_STATIC::COUNT); i++)
        {
            if (ComboList[selectedCombo] == ComboObjectList[i])
            {
                comboNumForItm = i;
                break;
            }
        }

        if (!m_vecObjectContainer[comboNumForItm])
        {
            //bNotFoundLoad = true;
        }
        else
        {
            string a = " (";
            string b = ")";
            string userInputName = ComboObjectList[comboNumForItm] + a + to_string(m_mapObjCount[static_cast<TAG_RES_STATIC>(comboNumForItm)] + 1) + b;
            sprintf_s(buf1, userInputName.c_str());
            bLoadButton = true;
        }



    }
    if (bLoadButton)
    {
        ImGui::OpenPopup("Please name your Object");
        if (ImGui::BeginPopupModal("Please name your Object", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {

            ImGui::InputText("", buf1, 64);
            static bool bAlreadyHaveName = false;
            if (bAlreadyHaveName)
                ImGui::Text("Already Have that Name, Please Enter new Name");
            ImGui::Separator();
            if (ImGui::Button("OK", ImVec2(120, 0))) {

                if (m_mapObject.find(buf1) == m_mapObject.end())
                {
                    ObjectLoader(comboNumForItm, buf1);
                    hierarchySelectedColliderIndex = -1;
                    hierarchySelectedObjIndex = m_mapObject.size() - 1;
                    bLoadButton = false;
                    bAlreadyHaveName = false;
                }
                else
                {
                    bAlreadyHaveName = true;
                }
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                bLoadButton = false;
                bAlreadyHaveName = false;
            }

        }
        ImGui::EndPopup();
    }
    //if (bNotFoundLoad)
    //{
    //    ImGui::OpenPopup("Object Not Found");
    //    if (ImGui::BeginPopupModal("Sorry, Cannot found Object", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    //    {

    //        //ImGui::InputText("", buf1, 64);
    //        //static bool bAlreadyHaveName = false;
    //        //if (bAlreadyHaveName)
    //        //    ImGui::Text("Already Have that Name, Please Enter new Name");
    //        //ImGui::Separator();
    //        //if (ImGui::Button("OK", ImVec2(120, 0))) {

    //        //    if (m_mapObject.find(buf1) == m_mapObject.end())
    //        //    {
    //        //        ObjectLoader(comboNum, buf1);
    //        //        hierarchySelectedColliderIndex = -1;
    //        //        hierarchySelectedObjIndex = m_mapObject.size() - 1;
    //        //        bLoadButton = false;
    //        //        bAlreadyHaveName = false;
    //        //    }
    //        //    else
    //        //    {
    //        //        bAlreadyHaveName = true;
    //        //    }
    //        //}
    //        //ImGui::SetItemDefaultFocus();
    //        //ImGui::SameLine();
    //        if (ImGui::Button("Cancel", ImVec2(120, 0)))
    //        {
    //            bNotFoundLoad = false;
    //        }
    //    }
    //    ImGui::EndPopup();
    //}
}

BoxColliderInFile::BoxColliderInFile()
{
    D3DXMatrixIdentity(&m_transform);
}

ObjectInFile::ObjectInFile()
    : m_tagResStatic(TAG_RES_STATIC::Bandage)
    , m_name("")
    , m_position(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
    , m_rotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
    , m_scale(D3DXVECTOR3(1.0f, 1.0f, 1.0f))
{
}