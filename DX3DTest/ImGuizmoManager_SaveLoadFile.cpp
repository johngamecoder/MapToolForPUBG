#include "stdafx.h"
#include "ImGuizmoManager.h"
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <chrono>
#include <ctime>
//#include "BoxCollider.h"

void ImGuizmoManager::MenuBarImGui()
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        //ImGui::MenuItem("(dummy menu)", NULL, false, false);
        if (ImGui::MenuItem("New Scene")) Init();


        if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
        {
            string fullPath = "../../TeamAvengers_PUBG/PUBG/Client/Resource/save.txt";
            OpenScene2(fullPath);
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
            string fileName = "../../TeamAvengers_PUBG/PUBG/Client/Resource/save.txt";
            SaveScene(fileName);
            




            
            //Ȥ�� ���� ������ ����� �س��Ҵ�
            string backupPath = "./Resource/SaveBackUp/UpdateBackup/";

            time_t now_time;
            char buf[256];
            time(&now_time); //���� �� ���� �ð��� ����
            ctime_s(buf, sizeof(buf), &now_time);
            
            stringstream ss;
            ss.str(buf);

            string namePath , buffer;
            while (ss)
            {
                ss >> buffer;
                namePath += buffer;
            }
            
            stringstream ss2;
            ss2.str(namePath);
            string token;
            string addup;
            while (getline(ss2, token,':'))
            {
                addup += token;
            }

            backupPath += addup;
            backupPath += ".txt";
            SaveScene(backupPath);
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
//void ImGuizmoManager::OpenScene(const string& fileName)
//{
//    Init(); // clearing the scene
//
//    std::ifstream myFile(fileName);
//    if (myFile.is_open())
//    {
//        string line;
//        getline(myFile, line);
//        if (line != fileName)
//            assert(false && "Wrong file loaded");
//
//        m_currentSceneName = line;  //opening current scene 
//        getline(myFile, line); //take out "Num of Object n"
//
//        while (!myFile.eof())
//        {
//            getline(myFile, line); //take out {
//            if (line != "{") break;
//            std::stringstream ss;
//            std::stringstream ssName;
//            std::stringstream ssMat;
//
//            ObjInfo* temp = new ObjInfo();
//            getline(myFile, line);                      temp->list = static_cast<TAG_RES_STATIC>(stoi(line));
//            getline(myFile, line);      ssName << line;     
///*���� ���ľߵ�! ��� /t�� ������?*/            string n,t; ssName >> n;ssName >> t;        temp->m_ObjName = n+" "+t;
//
//            getline(myFile, line);      ss << line;     ss >> line;     temp->m_Position.x = stof(line, 0);     ss >> line;     temp->m_Position.y = stof(line, 0);      ss >> line;    temp->m_Position.z = stof(line, 0);
//            getline(myFile, line);      ss << line;     ss >> line;     temp->m_Rotation.x = stof(line, 0);     ss >> line;     temp->m_Rotation.y = stof(line, 0);      ss >> line;    temp->m_Rotation.z = stof(line, 0);
//            getline(myFile, line);      ss << line;     ss >> line;     temp->m_Scale.x = stof(line, 0);        ss >> line;     temp->m_Scale.y = stof(line, 0);         ss >> line;    temp->m_Scale.z = stof(line, 0);
//
//            //���� matrix�� �޴� �ſ�����, �̰� ������� ���� ���� ������ matrix�� ����� ����
//            //getline(myFile, line);      ssMat << line;
//            //ssMat >> line; temp->m_matTransform._11 = stof(line, 0);  ssMat >> line; temp->m_matTransform._12 = stof(line, 0);  ssMat >> line; temp->m_matTransform._13 = stof(line, 0);  ssMat >> line; temp->m_matTransform._14 = stof(line, 0);
//            //ssMat >> line; temp->m_matTransform._21 = stof(line, 0);  ssMat >> line; temp->m_matTransform._22 = stof(line, 0);  ssMat >> line; temp->m_matTransform._23 = stof(line, 0);  ssMat >> line; temp->m_matTransform._24 = stof(line, 0);
//            //ssMat >> line; temp->m_matTransform._31 = stof(line, 0);  ssMat >> line; temp->m_matTransform._32 = stof(line, 0);  ssMat >> line; temp->m_matTransform._33 = stof(line, 0);  ssMat >> line; temp->m_matTransform._34 = stof(line, 0);
//            //ssMat >> line; temp->m_matTransform._41 = stof(line, 0);  ssMat >> line; temp->m_matTransform._42 = stof(line, 0);  ssMat >> line; temp->m_matTransform._43 = stof(line, 0);  ssMat >> line; temp->m_matTransform._44 = stof(line, 0);
//            D3DXMATRIXA16 matS, matR, matT;
//            D3DXMatrixScaling(&matS, temp->m_Scale.x, temp->m_Scale.y, temp->m_Scale.z);
//            D3DXMatrixRotationYawPitchRoll(&matR, temp->m_Rotation.y, temp->m_Rotation.x, temp->m_Rotation.z);
//            D3DXMatrixTranslation(&matT, temp->m_Position.x, temp->m_Position.y, temp->m_Position.z);
//            temp->m_matTransform = matS * matR * matT;
//            
//            temp->objPtr= m_vecObjectContainer[static_cast<int>(temp->list)];
//
//            std::stringstream ssTakeTabOut;
//            getline(myFile, line); //check if it is "}" or "["
//            ssTakeTabOut << line;
//            ssTakeTabOut >> line;
//            
//            if (line == "[")
//            {
//                
//                getline(myFile, line);
//                int colliderNum = atoi(line.c_str());
//                for (int i = 0; i < colliderNum; i++)
//                {
//                    std::stringstream ssNomore;
//                    getline(myFile, line);  //take out "("
//                    getline(myFile, line);  //get parent name;
//                    BoxCollider* bc = new BoxCollider(&temp->m_ObjName);
//                    bc->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), D3DXVECTOR3(5.0f, 5.0f, 5.0f));
//
//                    //getline(myFile, line);      ss << line;     ss >> line;    bc->m_vCenter.x = stof(line, 0);     ss >> line;     bc->m_vCenter.y = stof(line, 0);      ss >> line;    bc->m_vCenter.z = stof(line, 0);
//                    //getline(myFile, line);      ss << line;     ss >> line;    bc->m_vExtent.x = stof(line, 0);     ss >> line;     bc->m_vExtent.y = stof(line, 0);      ss >> line;    bc->m_vExtent.z = stof(line, 0);
//
//                    getline(myFile, line);      ssNomore << line;
//                    ssNomore >> line; bc->m_matWorld._11 = stof(line, 0);  ssNomore >> line; bc->m_matWorld._12 = stof(line, 0);  ssNomore >> line; bc->m_matWorld._13 = stof(line, 0);  ssNomore >> line; bc->m_matWorld._14 = stof(line, 0);
//                    ssNomore >> line; bc->m_matWorld._21 = stof(line, 0);  ssNomore >> line; bc->m_matWorld._22 = stof(line, 0);  ssNomore >> line; bc->m_matWorld._23 = stof(line, 0);  ssNomore >> line; bc->m_matWorld._24 = stof(line, 0);
//                    ssNomore >> line; bc->m_matWorld._31 = stof(line, 0);  ssNomore >> line; bc->m_matWorld._32 = stof(line, 0);  ssNomore >> line; bc->m_matWorld._33 = stof(line, 0);  ssNomore >> line; bc->m_matWorld._34 = stof(line, 0);
//                    ssNomore >> line; bc->m_matWorld._41 = stof(line, 0);  ssNomore >> line; bc->m_matWorld._42 = stof(line, 0);  ssNomore >> line; bc->m_matWorld._43 = stof(line, 0);  ssNomore >> line; bc->m_matWorld._44 = stof(line, 0);
//
//                    bc->m_mParentTransform = temp->m_matTransform;
//                    D3DXMATRIXA16 mInvParent,check;
//                    D3DXMatrixInverse(&mInvParent, NULL, &bc->m_mParentTransform);
//                    bc->m_mTransform = bc->m_matWorld * mInvParent;
//
//
//                    temp->m_vecBoxCollider.push_back(bc);
//                    getline(myFile, line);//take out ")";
//
//                }
//                getline(myFile, line); //take out "]"
//                getline(myFile, line);  //take out "}"
//            }
//
//
//
//
//
//            //temp->objPtr = m_vecObjectContainer[temp->list];
//
//
//            m_mapObject.emplace(temp->m_ObjName, temp);
//            //m_mapObject.emplace_back(make_pair(temp->m_ObjName, temp));
//        }
//        myFile.close();
//    }
//    else
//    {
//        assert(false && "�ȵ�.�̤�");
//    }
//
//    for (auto p : m_mapObject)
//    {
//        if (m_mapObjCount.find(p.second->list) == m_mapObjCount.end())
//        {
//            m_mapObjCount.emplace(p.second->list, 0);
//        }
//        m_mapObjCount[p.second->list]++;
//    }
//}
void ImGuizmoManager::OpenScene2(const string& fullPath)
{
    Init(); // clearing the scene

    std::ifstream fin(fullPath);

    assert(fin.is_open() &&
        "ImGuizmoManager::OpenScene(), file path is invalid.");

    string filename;
    fin >> filename;

    string buf;
    fin >> buf >> buf >> buf;   // Num of Object
    int numObjects;
    fin >> numObjects;

    HRESULT hr;
    std::vector<ObjectInFile> objs;
    for (int oi = 0; oi < numObjects; ++oi)
    {
        ObjectInFile obj;
        hr = parseObjectInFile(fin, &obj);
        if (FAILED(hr))
        {
            assert(false &&
                "IScene::LoadObjectsFromFile(), parsing failed.");
        }

        objs.emplace_back(obj);
    }

    for (auto o : objs)
    {
        // degrees to radians
        o.m_rotation = D3DXToRadian(o.m_rotation);


        ObjInfo* obj = new ObjInfo;
        obj->list = o.m_tagResStatic;
        
        obj->m_ObjName = o.m_name;
        
        obj->m_Position = o.m_position;
        obj->m_Rotation = o.m_rotation;
        obj->m_Scale = o.m_scale;


        
        D3DXMATRIXA16 s, r, t;
        D3DXMatrixScaling(&s, obj->m_Scale.x, obj->m_Scale.y, obj->m_Scale.z);
        D3DXMatrixRotationYawPitchRoll(&r, obj->m_Rotation.y, obj->m_Rotation.x, obj->m_Rotation.z);
        D3DXMatrixTranslation(&t, obj->m_Position.x, obj->m_Position.y, obj->m_Position.z);
        obj->m_matTransform = s * r * t;
        
        // radians to degrees
        obj->m_Rotation = D3DXToDegree(obj->m_Rotation);

        obj->objPtr = m_vecObjectContainer[static_cast<int>(obj->list)];
        


        for (auto boxcol : o.m_boxColliders)
        {
            BoxCollider* bc = new BoxCollider(&obj->m_ObjName);
            bc->Init(D3DXVECTOR3(-5.0f, -5.0f, -5.0f), D3DXVECTOR3(5.0f, 5.0f, 5.0f));
            bc->m_matWorld = boxcol.m_transform;
            bc->m_mParentTransform = obj->m_matTransform;
            
            D3DXMATRIXA16 mInvParent, check;
            D3DXMatrixInverse(&mInvParent, NULL, &bc->m_mParentTransform);
            bc->m_mTransform = bc->m_matWorld * mInvParent;

            obj->m_vecBoxCollider.push_back(bc);
        }
        
        m_mapObject.emplace(obj->m_ObjName, obj);
    }
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
        assert(false && "�ȵ�..");
    }

}

HRESULT ImGuizmoManager::parseObjectInFile(std::ifstream& fin, ObjectInFile* Out)
{
    assert(Out && "IScene::parseObjectInFile(), out is null.");

    string buf;
    fin >> buf;
    if (buf != "{")
    {
        return E_FAIL;
    }

    fin >> buf;
    Out->m_tagResStatic = static_cast<TAG_RES_STATIC>(std::stoi(buf));

    std::getline(fin >> std::ws, buf);
    Out->m_name = buf;

    fin >> Out->m_position.x >> Out->m_position.y >> Out->m_position.z
        >> Out->m_rotation.x >> Out->m_rotation.y >> Out->m_rotation.z
        >> Out->m_scale.x >> Out->m_scale.y >> Out->m_scale.z;

    fin >> buf;
    if (buf == "}")
    {
        return S_OK;
    }
    else if (buf != "[")
    {
        return E_FAIL;
    }

    HRESULT hr;
    fin >> buf;
    for (int ci = 0; ci < std::stoi(buf); ++ci)
    {
        BoxColliderInFile box;
        hr = parseBoxColliderInFile(fin, &box);
        if (FAILED(hr))
        {
            return E_FAIL;
        }

        Out->m_boxColliders.emplace_back(box);
    }

    fin >> buf;
    if (buf != "]")
    {
        return E_FAIL;
    }

    fin >> buf;
    if (buf != "}")
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT ImGuizmoManager::parseBoxColliderInFile(
    std::ifstream& fin, BoxColliderInFile* Out)
{
    assert(Out && "IScene::parseBoxColliderInFile(), out is null.");

    string buf;
    fin >> buf;
    if (buf != "<")
    {
        return E_FAIL;
    }

    std::getline(fin >> std::ws, buf);

    fin >> Out->m_transform._11 >> Out->m_transform._12
        >> Out->m_transform._13 >> Out->m_transform._14
        >> Out->m_transform._21 >> Out->m_transform._22
        >> Out->m_transform._23 >> Out->m_transform._24
        >> Out->m_transform._31 >> Out->m_transform._32
        >> Out->m_transform._33 >> Out->m_transform._34
        >> Out->m_transform._41 >> Out->m_transform._42
        >> Out->m_transform._43 >> Out->m_transform._44;

    fin >> buf;
    if (buf != ">")
    {
        return E_FAIL;
    }

    return S_OK;
}

