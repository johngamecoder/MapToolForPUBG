#include "stdafx.h"
#include "ImGuizmoManager.h"
#include <stdio.h>
#include <sstream>

void ImGuizmoManager::MenuBarImGui()
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        //ImGui::MenuItem("(dummy menu)", NULL, false, false);
        if (ImGui::MenuItem("New Scene")) Init();


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
            string fileName = "save.txt";
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
    //Init(); // clearing the scene

    //std::ifstream myFile(fileName);
    //if (myFile.is_open())
    //{
    //    string line;
    //    getline(myFile, line);
    //    if (line != fileName)
    //        assert(false && "Wrong file loaded");
    //    m_currentSceneName = line;  //opening current scene 
    //    getline(myFile, line); //take out "Num of Object n"

    //    while (!myFile.eof())
    //    {
    //        std::stringstream ss;
    //        std::stringstream ssMat;
    //        getline(myFile, line); //take out {
    //        if (line != "{") break;
    //        ObjInfo* temp = new ObjInfo();
    //        getline(myFile, line);                      temp->list = ObjList(stoi(line));
    //        getline(myFile, line);                      temp->m_ObjName += line;
    //        getline(myFile, line);      ss << line;     ss >> line;     temp->m_Position.x = stof(line, 0);     ss >> line;     temp->m_Position.y = stof(line, 0);      ss >> line;    temp->m_Position.z = stof(line, 0);
    //        getline(myFile, line);      ss << line;     ss >> line;     temp->m_Rotation.x = stof(line, 0);     ss >> line;     temp->m_Rotation.y = stof(line, 0);      ss >> line;    temp->m_Rotation.z = stof(line, 0);
    //        getline(myFile, line);      ss << line;     ss >> line;     temp->m_Scale.x = stof(line, 0);        ss >> line;     temp->m_Scale.y = stof(line, 0);         ss >> line;    temp->m_Scale.z = stof(line, 0);

    //        //원래 matrix를 받는 거였지만, 이걸 사용하지 말고 위에 것으로 matrix를 만들어 주자
    //        //getline(myFile, line);      ssMat << line;
    //        //ssMat >> line; temp->m_matTransform._11 = stof(line, 0);  ssMat >> line; temp->m_matTransform._12 = stof(line, 0);  ssMat >> line; temp->m_matTransform._13 = stof(line, 0);  ssMat >> line; temp->m_matTransform._14 = stof(line, 0);
    //        //ssMat >> line; temp->m_matTransform._21 = stof(line, 0);  ssMat >> line; temp->m_matTransform._22 = stof(line, 0);  ssMat >> line; temp->m_matTransform._23 = stof(line, 0);  ssMat >> line; temp->m_matTransform._24 = stof(line, 0);
    //        //ssMat >> line; temp->m_matTransform._31 = stof(line, 0);  ssMat >> line; temp->m_matTransform._32 = stof(line, 0);  ssMat >> line; temp->m_matTransform._33 = stof(line, 0);  ssMat >> line; temp->m_matTransform._34 = stof(line, 0);
    //        //ssMat >> line; temp->m_matTransform._41 = stof(line, 0);  ssMat >> line; temp->m_matTransform._42 = stof(line, 0);  ssMat >> line; temp->m_matTransform._43 = stof(line, 0);  ssMat >> line; temp->m_matTransform._44 = stof(line, 0);
    //        D3DXMATRIXA16 matS, matR, matT;
    //        D3DXMatrixScaling(&matS, temp->m_Scale.x, temp->m_Scale.y, temp->m_Scale.z);
    //        D3DXMatrixRotationYawPitchRoll(&matR, temp->m_Rotation.y, temp->m_Rotation.x, temp->m_Rotation.z);
    //        D3DXMatrixTranslation(&matT, temp->m_Position.x, temp->m_Position.y, temp->m_Position.z);
    //        temp->m_matTransform = 

    //        getline(myFile, line);
    //        if (line == "[")
    //        {
    //            getline(myFile, line);
    //            int colliderNum = atoi(line.c_str());
    //            for (int i = 0; i < colliderNum; i++)
    //            {
    //                getline(myFile, line);  //take out "("
    //                getline(myFile, line);  //get parent name;
    //                BoxCollider* bc = new BoxCollider(line);
    //                bc->Init(D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DXVECTOR3(0.5f, 0.5f, 0.5f));

    //                //getline(myFile, line);      ss << line;     ss >> line;    bc->m_vCenter.x = stof(line, 0);     ss >> line;     bc->m_vCenter.y = stof(line, 0);      ss >> line;    bc->m_vCenter.z = stof(line, 0);
    //                //getline(myFile, line);      ss << line;     ss >> line;    bc->m_vExtent.x = stof(line, 0);     ss >> line;     bc->m_vExtent.y = stof(line, 0);      ss >> line;    bc->m_vExtent.z = stof(line, 0);

    //                getline(myFile, line);      ssMat << line;
    //                ssMat >> line; bc->m_mTransform._11 = stof(line, 0);  ssMat >> line; bc->m_mTransform._12 = stof(line, 0);  ssMat >> line; bc->m_mTransform._13 = stof(line, 0);  ssMat >> line; bc->m_mTransform._14 = stof(line, 0);
    //                ssMat >> line; bc->m_mTransform._21 = stof(line, 0);  ssMat >> line; bc->m_mTransform._22 = stof(line, 0);  ssMat >> line; bc->m_mTransform._23 = stof(line, 0);  ssMat >> line; bc->m_mTransform._24 = stof(line, 0);
    //                ssMat >> line; bc->m_mTransform._31 = stof(line, 0);  ssMat >> line; bc->m_mTransform._32 = stof(line, 0);  ssMat >> line; bc->m_mTransform._33 = stof(line, 0);  ssMat >> line; bc->m_mTransform._34 = stof(line, 0);
    //                ssMat >> line; bc->m_mTransform._41 = stof(line, 0);  ssMat >> line; bc->m_mTransform._42 = stof(line, 0);  ssMat >> line; bc->m_mTransform._43 = stof(line, 0);  ssMat >> line; bc->m_mTransform._44 = stof(line, 0);

    //                temp->m_vecBoxCollider.push_back(bc);
    //                getline(myFile, line);//take out ")";

    //            }
    //            getline(myFile, line); //take out "]"
    //            getline(myFile, line);  //take out "}"
    //        }
    //        else
    //        {
    //            getline(myFile, line);  //take out "}"
    //        }




    //        temp->objPtr = m_vecObjectContainer[temp->list];


    //        m_mapObject.emplace(temp->m_ObjName, temp);
    //        //m_mapObject.emplace_back(make_pair(temp->m_ObjName, temp));
    //    }
    //    myFile.close();
    //}
    //else
    //{
    //    assert(false && "안됨.ㅜㅜ");
    //}

    //for (auto p : m_mapObject)
    //{
    //    if (m_mapObjCount.find(p.second->list) == m_mapObjCount.end())
    //    {
    //        m_mapObjCount.emplace(p.second->list, 0);
    //    }
    //    m_mapObjCount[p.second->list]++;
    //}
}
void ImGuizmoManager::SaveScene(string& fileName)
{
    std::ofstream myFile(fileName);
    if (myFile.is_open())
    {
        myFile << fileName << "\n";
        myFile << "Num of Object " << m_mapObject.size() << "\n";

        for (auto p : m_mapObject)
        {
            ObjInfo* temp = p.second;
            myFile << "{" << "\n";
            myFile << "\t"<< to_string(static_cast<int>(temp->list)) + "\n";
            myFile << "\t"<< temp->m_ObjName + "\n";
            myFile << "\t"<< to_string(temp->m_Position.x) + " " + to_string(temp->m_Position.y) + " " + to_string(temp->m_Position.z) + "\n";
            myFile << "\t"<< to_string(temp->m_Rotation.x) + " " + to_string(temp->m_Rotation.y) + " " + to_string(temp->m_Rotation.z) + "\n";
            myFile << "\t"<< to_string(temp->m_Scale.x) + " " + to_string(temp->m_Scale.y) + " " + to_string(temp->m_Scale.z) + "\n";
            //myFile << "\t"<< to_string(temp->m_matTransform._11) + " " + to_string(temp->m_matTransform._12) + " " + to_string(temp->m_matTransform._13) + " " + to_string(temp->m_matTransform._14) + " "
            //    + to_string(temp->m_matTransform._21) + " " + to_string(temp->m_matTransform._22) + " " + to_string(temp->m_matTransform._23) + " " + to_string(temp->m_matTransform._24) + " "
            //    + to_string(temp->m_matTransform._31) + " " + to_string(temp->m_matTransform._32) + " " + to_string(temp->m_matTransform._33) + " " + to_string(temp->m_matTransform._34) + " "
            //    + to_string(temp->m_matTransform._41) + " " + to_string(temp->m_matTransform._42) + " " + to_string(temp->m_matTransform._43) + " " + to_string(temp->m_matTransform._44) + "\n";
            int colliderNum = temp->m_vecBoxCollider.size();
            if (colliderNum > 0)
            {
                myFile << "\t"<<"[" << "\n";
                myFile << "\t" << "\t" << colliderNum << "\n";
                for (int i = 0; i < colliderNum; i++)
                {
                    myFile << "\t" << "\t" <<"<" << "\n";
                    myFile << "\t" << "\t" << "\t" << *(temp->m_vecBoxCollider[i]->GetParentName()) << "\n";
                    //D3DXVECTOR3 vCenter = temp->m_vecBoxCollider[i]->GetCenter();
                    //D3DXVECTOR3 vExtent = temp->m_vecBoxCollider[i]->GetExtent();
                    //myFile << "\t" << "\t" << "\t" << to_string(vCenter.x) + " " + to_string(vCenter.y) + " " + to_string(vCenter.z) + "\n";
                    //myFile << "\t" << "\t" << "\t" << to_string(vExtent.x) + " " + to_string(vExtent.y) + " " + to_string(vExtent.z) + "\n";
                    myFile << "\t" << "\t" << "\t" << to_string(temp->m_vecBoxCollider[i]->m_matWorld._11) + " " + to_string(temp->m_vecBoxCollider[i]->m_matWorld._12) + " " + to_string(temp->m_vecBoxCollider[i]->m_matWorld._13) + " " + to_string(temp->m_vecBoxCollider[i]->m_matWorld._14) + " "
                                                    + to_string(temp->m_vecBoxCollider[i]->m_matWorld._21) + " " + to_string(temp->m_vecBoxCollider[i]->m_matWorld._22) + " " + to_string(temp->m_vecBoxCollider[i]->m_matWorld._23) + " " + to_string(temp->m_vecBoxCollider[i]->m_matWorld._24) + " "
                                                    + to_string(temp->m_vecBoxCollider[i]->m_matWorld._31) + " " + to_string(temp->m_vecBoxCollider[i]->m_matWorld._32) + " " + to_string(temp->m_vecBoxCollider[i]->m_matWorld._33) + " " + to_string(temp->m_vecBoxCollider[i]->m_matWorld._34) + " "
                                                    + to_string(temp->m_vecBoxCollider[i]->m_matWorld._41) + " " + to_string(temp->m_vecBoxCollider[i]->m_matWorld._42) + " " + to_string(temp->m_vecBoxCollider[i]->m_matWorld._43) + " " + to_string(temp->m_vecBoxCollider[i]->m_matWorld._44) + "\n";

                    myFile << "\t" << "\t" << ">" << "\n";
                }
                myFile << "\t" <<"]" << "\n";
            }

            myFile << "}" << "\n";
        }
        myFile.close();
    }
    else
    {
        assert(false && "안됨..");
    }

}



