#include "stdafx.h"
#include "ImGuizmoManager.h"

#include "PUBG_Object.h"

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
        m_mapCount.emplace((ObjList)i, 0);
    }
}

void ImGuizmoManager::ObjectLoader(int index)
{
    ObjInfo* temp = new ObjInfo(++m_mapCount[(ObjList)index], (ObjList)index);
    temp->objPtr = m_vecObjectContainer[(ObjList)index];
    temp->m_ObjName = m_vecObjectContainer[(ObjList)index]->name + to_string(m_mapCount[(ObjList)index]);
    //m_mapObject.emplace(temp->m_ObjName, temp);

    //
    m_mapObject.emplace_back(make_pair(temp->m_ObjName, temp));
    //

    SetCurrentObject(temp);
}