#include "stdafx.h"
#include "ImGuizmoManager.h"
#include "ResourceInfo.h"
#include "PUBG_Object.h"

//const char* ComboObjectList[(unsigned int)TAG_RES_STATIC::COUNT];// = { "Bandage","Church"/*,"Tree","Rock","Ware House"*/ };
char* ComboObjectList[static_cast<int>(TAG_RES_STATIC::COUNT)];

void ImGuizmoManager::LoadObjectImGui()
{
    ImGui::SetNextWindowPos(ImVec2(30, 500));
    ImGui::Begin("Object Loader");
    {
        ImGui::Combo("", &comboSelect, ComboObjectList, static_cast<int>(TAG_RES_STATIC::COUNT));
        //ImGui::Combo("", &comboSelect, ComboObjectList, LOADCOUNT/*ObjList::COUNT*//*이건 갯수 넣는 부분 */);
        ImGui::SameLine();
        

        static char buf1[64];
        static bool bLoadButton = false;
        if (ImGui::Button("Load"))
        {
            if (!m_vecObjectContainer[comboSelect])
            {
                assert(false && "no file");
            }
            string a = " (";
            string b = ")";
            string userInputName = ComboObjectList[comboSelect] + a + to_string(m_mapObjCount[static_cast<TAG_RES_STATIC>(comboSelect)] + 1) + b;
            sprintf_s(buf1, userInputName.c_str());
            bLoadButton = true;

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
                        ObjectLoader(comboSelect, buf1);
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

        ImGui::Separator();
        if (m_pCurrentObject)
        {
            if (hierarchySelectedObjIndex != -1)
            {
                ImGui::Text("You have selected : ");
                char buf[50];
                //string ment = m_pCurrentObject->m_ObjName;
                sprintf_s(buf, m_pCurrentObject->m_ObjName.c_str());
                ImGui::Text(buf);
                if (ImGui::Button("Add Box Collider"))
                {
                    AddBoxCollider();
                }


                static bool bDeleteObject = false;
                if (ImGui::Button("Delete selected object"))
                {
                    bDeleteObject = true;
                }
                if (bDeleteObject)
                {
                    ImGui::OpenPopup("Delete Object");
                    if (ImGui::BeginPopupModal("Delete Object", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                    {
                        ImGui::Text("Are you Sure? \nThis operation cannot be undone!\n\n");

                        ImGui::Separator();

                        static bool dont_ask_me_next_time = false;
                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                        ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
                        ImGui::PopStyleVar();

                        if (ImGui::Button("OK", ImVec2(120, 0)))
                        {
                            DeleteObject();
                            bDeleteObject = false;
                        }
                        ImGui::SetItemDefaultFocus();
                        ImGui::SameLine();
                        if (ImGui::Button("Cancel", ImVec2(120, 0)))
                        {
                            bDeleteObject = false;
                        }

                        ImGui::EndPopup();
                    }
                }

                if (hierarchySelectedColliderIndex != -1)
                {
                    if (ImGui::Button("Delete selected Box Collider"))
                    {
                        
                    }
                }
            }
        }
        
    }ImGui::End();
}
void ImGuizmoManager::ConstructComboObjectList()
{
    for (int i = 0; i < static_cast<int>(TAG_RES_STATIC::COUNT); ++i)
    {
        if (ResourceInfo::IsItem(static_cast<TAG_RES_STATIC>(i)))
            continue;
        ComboObjectList[i] = new char[64]();
    }

    pair<string, string> PATHnNAME;
    int num = static_cast<int>(TAG_RES_STATIC::COUNT);
    for (int i = 0; i < num; i++)
    {
        //if (ResourceInfo::IsItem(static_cast<TAG_RES_STATIC>(i)))
        //    continue;
        PATHnNAME = ResourceInfo::GetFileNameWithoutX(static_cast<TAG_RES_STATIC>(i));
        memcpy_s(ComboObjectList[i], 64, PATHnNAME.second.c_str(), PATHnNAME.second.size());
        //ComboObjectList[i] = PATHnNAME.second.c_str();
    }
    
}
void ImGuizmoManager::ContainObject()
{  
    ////---testing 용 code-------------
    //m_vecObjectContainer.resize(LOADCOUNT);
    //pair<string, string> PATHnNAME;
    //PATHnNAME = ResPathFileName::Get(static_cast<TAG_RES_STATIC>(9));
    //m_vecObjectContainer[0] = new PUBG_Object(PATHnNAME.first, PATHnNAME.second);
    //m_vecObjectContainer[0]->Init();
    //m_mapObjCount.emplace(static_cast<TAG_RES_STATIC>(9), 0);
    //PATHnNAME = ResPathFileName::Get(static_cast<TAG_RES_STATIC>(16));
    //m_vecObjectContainer[1] = new PUBG_Object(PATHnNAME.first, PATHnNAME.second);
    //m_vecObjectContainer[1]->Init();
    //m_mapObjCount.emplace(static_cast<TAG_RES_STATIC>(16), 0);

    //------ 실제 사용할 코드! 지우지 마삼! -------------------
    pair<string, string> PATHnNAME;
    for (int i = 0; i < static_cast<int>(TAG_RES_STATIC::COUNT); i++)
    {
        if (i == 9 || i == 16)//<<<< 이거 나중에 빼야함! (지금은 
        {
            PATHnNAME = ResourceInfo::GetPathFileName(static_cast<TAG_RES_STATIC>(i));
            m_vecObjectContainer[i] = new PUBG_Object(PATHnNAME.first, PATHnNAME.second);
            if (!m_vecObjectContainer[i])
                assert(false && "Load x failed");
            m_vecObjectContainer[i]->Init();
            m_mapObjCount.emplace(static_cast<TAG_RES_STATIC>(i), 0);
        }
        else
        {
            m_vecObjectContainer[i] = nullptr;
        }
    }
    //------ 실제 사용할 코드! 지우지 마삼! -------------------
}

void ImGuizmoManager::ObjectLoader(const int index,const string& userInputName)
{
    ObjInfo* temp = new ObjInfo(static_cast<TAG_RES_STATIC>(index), userInputName);
    temp->objPtr = m_vecObjectContainer[index];
    m_mapObjCount[static_cast<TAG_RES_STATIC>(index)]++;
    m_mapObject.emplace(temp->m_ObjName, temp);
    //m_mapObject.emplace_back(make_pair(temp->m_ObjName, temp));


    SetCurrentObject(temp);
}
void ImGuizmoManager::DeleteObject()
{
    ObjInfo* temp = m_pCurrentObject;
    m_mapObjCount[temp->list]--;
    m_pCurrentObject = NULL;

    m_mapObject.erase(temp->m_ObjName); //delete from map

    for (int i = 0; i < temp->m_vecBoxCollider.size(); i++)
    {
        SAFE_DELETE(temp->m_vecBoxCollider[i]);
    }
    temp->m_vecBoxCollider.clear();
    SAFE_DELETE(temp);

    mCurrentGizmoOperation = ImGuizmo::NOTSELECTED;
}