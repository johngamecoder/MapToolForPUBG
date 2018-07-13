#include "stdafx.h"
#include "ImGuizmoManager.h"
#include "ResourceInfo.h"
#include "PUBG_Object.h"

//const char* ComboObjectList[(unsigned int)TAG_RES_STATIC::COUNT];// = { "Bandage","Church"/*,"Tree","Rock","Ware House"*/ };
char* ComboObjectList[static_cast<int>(TAG_RES_STATIC::COUNT)];
char* ComboTerrainFeaturesList[63];
char* ComboItemsList[13];

void ImGuizmoManager::LoadObjectImGui()
{
    ImGui::SetNextWindowPos(ImVec2(30, 500));
    ImGui::Begin("Object Loader");
    {
        ImGui::Text("Terrain Features");
        //ImGui::Combo("", &comboSelect, ComboObjectList, LOADCOUNT/*ObjList::COUNT*//*이건 갯수 넣는 부분 */);
        //ImGui::Combo("", &comboSelect, ComboObjectList, static_cast<int>(TAG_RES_STATIC::COUNT));
        ImGui::Combo("", &comboTerrainFeatureSelect, ComboTerrainFeaturesList, 63);
        ImGui::SameLine();

        ObjectLoaderButton(ComboTerrainFeaturesList,comboTerrainFeatureSelect);


        ImGui::Text("Items");
        ImGui::Combo("", &comboItemSelect, ComboItemsList, 13);
        ImGui::SameLine();

        ObjectItemLoaderButton(ComboItemsList,comboItemSelect);


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
        ComboObjectList[i] = new char[64]();
    }

    pair<string, string> PATHnNAME;
    int num = static_cast<int>(TAG_RES_STATIC::COUNT);
    int itemcount = 0;
    int terraincount = 0;
    for (int i = 0; i < num; i++)
    {
        PATHnNAME = ResourceInfo::GetFileNameWithoutX(static_cast<TAG_RES_STATIC>(i));
        memcpy_s(ComboObjectList[i], 64, PATHnNAME.second.c_str(), PATHnNAME.second.size());
        if (ResourceInfo::IsItem(static_cast<TAG_RES_STATIC>(i)))
        {
            ComboItemsList[itemcount++] = ComboObjectList[i];
        }
        else
        {
            ComboTerrainFeaturesList[terraincount++] = ComboObjectList[i];
        }
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
        if (i==0||i==22)//<<<< 이거 나중에 빼야함! (지금은 
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