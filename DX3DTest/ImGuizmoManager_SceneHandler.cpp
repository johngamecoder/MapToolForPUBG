#include "stdafx.h"
#include "ImGuizmoManager.h"
#include <stdio.h>
#include <sstream>


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

                    //getline(myFile, line);      ss << line;     ss >> line;    bc->m_vCenter.x = stof(line, 0);     ss >> line;     bc->m_vCenter.y = stof(line, 0);      ss >> line;    bc->m_vCenter.z = stof(line, 0);
                    //getline(myFile, line);      ss << line;     ss >> line;    bc->m_vExtent.x = stof(line, 0);     ss >> line;     bc->m_vExtent.y = stof(line, 0);      ss >> line;    bc->m_vExtent.z = stof(line, 0);

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
        assert(false && "¾ÈµÊ.¤Ì¤Ì");
    }

    for (auto p : m_mapObject)
    {
        if (m_mapCount.find(p.second->list) == m_mapCount.end())
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
        myFile << "Num of Object :" << m_mapObject.size() << "\n";

        for (auto p : m_mapObject)
        {
            ObjInfo* temp = p.second;
            myFile << "{" << "\n";
            myFile << to_string(temp->ID) + "\n";
            myFile << to_string(temp->list) + "\n";
            myFile << temp->m_ObjName + "\n";
            myFile << to_string(temp->m_Position.x) + " " + to_string(temp->m_Position.y) + " " + to_string(temp->m_Position.z) + "\n";
            myFile << to_string(temp->m_Rotation.x) + " " + to_string(temp->m_Rotation.y) + " " + to_string(temp->m_Rotation.z) + "\n";
            myFile << to_string(temp->m_Scale.x) + " " + to_string(temp->m_Scale.y) + " " + to_string(temp->m_Scale.z) + "\n";
            myFile << to_string(temp->m_matTransform._11) + " " + to_string(temp->m_matTransform._12) + " " + to_string(temp->m_matTransform._13) + " " + to_string(temp->m_matTransform._14) + " "
                + to_string(temp->m_matTransform._21) + " " + to_string(temp->m_matTransform._22) + " " + to_string(temp->m_matTransform._23) + " " + to_string(temp->m_matTransform._24) + " "
                + to_string(temp->m_matTransform._31) + " " + to_string(temp->m_matTransform._32) + " " + to_string(temp->m_matTransform._33) + " " + to_string(temp->m_matTransform._34) + " "
                + to_string(temp->m_matTransform._41) + " " + to_string(temp->m_matTransform._42) + " " + to_string(temp->m_matTransform._43) + " " + to_string(temp->m_matTransform._44) + "\n";
            int colliderNum = temp->m_vecBoxCollider.size();
            if (colliderNum > 0)
            {
                myFile << "[" << "\n";
                myFile << colliderNum << "\n";
                for (int i = 0; i < colliderNum; i++)
                {
                    myFile << "(" << "\n";
                    myFile << temp->m_vecBoxCollider[i]->m_Parentname << "\n";
                    //myFile << to_string(temp->m_vecBoxCollider[i]->m_vCenter.x) + " " + to_string(temp->m_vecBoxCollider[i]->m_vCenter.y) + " " + to_string(temp->m_vecBoxCollider[i]->m_vCenter.z) + "\n";
                    //myFile << to_string(temp->m_vecBoxCollider[i]->m_vExtent.x) + " " + to_string(temp->m_vecBoxCollider[i]->m_vExtent.y) + " " + to_string(temp->m_vecBoxCollider[i]->m_vExtent.z) + "\n";
                    myFile << to_string(temp->m_vecBoxCollider[i]->m_mTransform._11) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._12) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._13) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._14) + " "
                        + to_string(temp->m_vecBoxCollider[i]->m_mTransform._21) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._22) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._23) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._24) + " "
                        + to_string(temp->m_vecBoxCollider[i]->m_mTransform._31) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._32) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._33) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._34) + " "
                        + to_string(temp->m_vecBoxCollider[i]->m_mTransform._41) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._42) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._43) + " " + to_string(temp->m_vecBoxCollider[i]->m_mTransform._44) + "\n";

                    myFile << ")" << "\n";
                }
                myFile << "]" << "\n";
            }

            myFile << "}" << "\n";
        }
        myFile.close();
    }
    else
    {
        assert(false && "¾ÈµÊ..");
    }

}

