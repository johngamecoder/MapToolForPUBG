#include "stdafx.h"
#include "ImGuizmoManager.h"
#include "ResourceInfo.h"
#include "PUBG_Object.h"

char* ComboObjectList[static_cast<int>(TAG_RES_STATIC::COUNT)];
char* ComboTerrainFeaturesList[63];
char* ComboItemsList[13];

void ImGuizmoManager::LoadObjectImGui()
{
    ImGui::SetNextWindowPos(ImVec2(30, 500));
    ImGui::Begin("Object Loader");
    {  
        static int e = 0;
        ImGui::RadioButton("Terrain Features", &e, 0); ImGui::SameLine();
        ImGui::RadioButton("Items", &e, 1);
        if (e == 0)
        {
            ImGui::Combo("", &comboTerrainFeatureSelect, ComboTerrainFeaturesList, 63);
            {
                ImGui::SameLine();
                ObjectLoaderButton(ComboTerrainFeaturesList, comboTerrainFeatureSelect);
            }
        }
        else
        {
            ImGui::Combo("", &comboItemSelect, ComboItemsList, 13);
            {
                ImGui::SameLine();
                ObjectItemLoaderButton(ComboItemsList, comboItemSelect);
            }
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
                            hierarchySelectedObjIndex = -1;
                            hierarchySelectedColliderIndex = -1;
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
                    ImGui::Text("You have selected box collider : %f", hierarchySelectedColliderIndex);

                    static bool bDeleteCollider = false;
                    if (ImGui::Button("Delete selected Box Collider"))
                    {
                        bDeleteCollider = true;
                    }
                    if (bDeleteCollider)
                    {
                        ImGui::OpenPopup("Delete Box Collider");
                        if (ImGui::BeginPopupModal("Delete Box Collider", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                        {
                            ImGui::Text("Are you Sure? \nThis operation cannot be undone!\n\n");

                            ImGui::Separator();

                            static bool dont_ask_me_next_time = false;
                            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                            ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
                            ImGui::PopStyleVar();

                            if (ImGui::Button("OK", ImVec2(120, 0)))
                            {
                                DeleteBoxCollider(hierarchySelectedColliderIndex);
                                bDeleteCollider = false;
                                hierarchySelectedObjIndex = -1;
                                hierarchySelectedColliderIndex = -1;

                            }
                            ImGui::SetItemDefaultFocus();
                            ImGui::SameLine();
                            if (ImGui::Button("Cancel", ImVec2(120, 0)))
                            {
                                bDeleteCollider = false;
                            }

                            ImGui::EndPopup();

                        }
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
        if (
            //buildings (사용함)
             i == 15    //AbandonedTownHall,
            || i == 16  //church
            || i == 17  //Museum,
            || i == 18  //OldWoodenShed_1,
            || i == 19  //OldWoodenShed_2,
            || i == 20  //OldWoodenShed_3,
            || i == 21  //PoliceStation,
            || i == 22  //wareHouse_A
            || i == 23  //WareHouse_B,
            || i == 1
            ////Exterior

            //////- BrokenVehicle
            //|| i == 26//BrokenBus,
            //|| i == 27//BrokenCar,
            //|| i == 28//BrokenMeshTruck,
            //|| i == 29//BrokenPoliceCar,
            //|| i == 30//BrokenTractorGunnyBag,
            //|| i == 31//BrokenUaz3151,
            
            ////-Container (사용함)
            || i == 32//ContainerBox_A,
            || i == 33//ContainerBox_B,
            || i == 34//ContainerSmall_1,
            || i == 35//ContainerSmall_2,

            ////- HayBale (사용함)
            || i == 36//HayBale_1,
            || i == 37//HayBale_2,

            //////- MetalBarrel (사용함)
            || i == 38//MetalBarrel_Blue,
            || i == 39//MetalBarrel_Gray,
            || i == 40//MetalBarrel_Green,
            || i == 41//MetalBarrel_Red,

            ////- metalfence
            || i == 42//metalfence_a,
            || i == 43//metalfence_long,

            ////- PicketFence
            || i == 44//PicketFence_Short_A,
            || i == 45//PicketFence_Short_B,
            || i == 46//PicketFence_Short_C,
            
            ////- Powerline 전보대
            //|| i == 47  //Powerline_1,
            //|| i == 48  //Powerline_2,

            ////- RadioTower
            //|| i == 49  //RadioTower_1,
            //|| i == 50  //RadioTower_2,

            //////- SandBag
            //|| i == 51  //Sandbag_1,
            //|| i == 52  //Sandbag_2,

            ////- Silo   (사용함)
            || i == 53  //Silo_A,
            || i == 54  //Silo_B,
            || i == 56  //Lighthouse,

            ////////- Etc
            //|| i == 55  //CityStreetSign,
            //
            //|| i == 57  //MetalShelf,
            //|| i == 58  //TableSet,
            //|| i == 59  //Tower,
            )
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

void ImGuizmoManager::DeleteBoxCollider(int index)
{
    BoxCollider* bc = m_pCurrentObject->m_vecBoxCollider[index];
    m_pCurrentObject->m_vecBoxCollider[index] = NULL;
    m_pCurrentObject->m_vecBoxCollider.erase(m_pCurrentObject->m_vecBoxCollider.begin() + index);
    
    SAFE_DELETE(bc);

    m_pCurrentObject = NULL;
    mCurrentGizmoOperation = ImGuizmo::NOTSELECTED;
}
