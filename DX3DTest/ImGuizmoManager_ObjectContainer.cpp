#include "stdafx.h"
#include "ImGuizmoManager.h"

#include "PUBG_Object.h"
void ImGuizmoManager::LoadObjectImGui()
{
    ImGui::SetNextWindowPos(ImVec2(30, 500));
    ImGui::Begin("Object Loader");
    {

        ImGui::Combo("", &comboSelect, ComboObjectList, ObjList::COUNT/*이건 갯수 넣는 부분 */);
        ImGui::SameLine();
        

        static char buf1[64];
        static bool bLoadButton = false;
        if (ImGui::Button("Load"))
        {
            string a = " (";
            string b = ")";
            string userInputName = ComboObjectList[comboSelect] + a + to_string(m_mapObjCount[(ObjList)comboSelect] + 1) + b;
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
void ImGuizmoManager::ContainObject()
{
    m_vecObjectContainer.resize(ObjList::COUNT);
    //m_vecObjectContainer.resize(2);

    m_vecObjectContainer[0] = new PUBG_Object("./Resource/Bandage/", "Bandage.X");
    m_vecObjectContainer[1] = new PUBG_Object("./Resource/Church/", "Church.X");
    
    for (int i = 0; i < m_vecObjectContainer.size(); i++)
    {
        m_vecObjectContainer[i]->Init();
        m_vecObjectContainer[i]->name = ComboObjectList[i];
        m_mapObjCount.emplace((ObjList)i, 0);
    }
}

void ImGuizmoManager::ObjectLoader(const int index,const string& userInputName)
{
    ObjInfo* temp = new ObjInfo((ObjList)index, userInputName);
    temp->objPtr = m_vecObjectContainer[(ObjList)index];
    m_mapObjCount[(ObjList)index]++;
    m_mapObject.emplace(temp->m_ObjName, temp);
    //m_mapObject.emplace_back(make_pair(temp->m_ObjName, temp));


    SetCurrentObject(temp);
}