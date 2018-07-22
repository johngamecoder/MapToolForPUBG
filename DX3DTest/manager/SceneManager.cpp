#include "stdafx.h"
#include "SceneManager.h"
#include "./scene/SceneGrid.h"
#include "./scene/SceneHeightmap.h"
#include "./scene/SceneTest.h"
#include "SceneMapTool.h"

SceneManager* SceneManager::instance = NULL;

SceneManager * SceneManager::Get()
{
    if (instance == NULL)
        instance = new SceneManager();

    return instance;
}

void SceneManager::Delete()
{
    SAFE_DELETE(instance);
}

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
    for (auto p : m_vecScene)
    {
        SAFE_RELEASE(p);
    }
}

void SceneManager::Init()
{
    //m_vecScene.push_back(new SceneGrid());
    //m_vecScene.push_back(new SceneObjMap());
    //m_vecScene.push_back(new SceneHeightmap());
    //m_vecScene.push_back(new SceneTest());
    m_vecScene.push_back(new SceneMapTool());
    //SetCurrentScene(SCENE_GRID);
    //SetCurrentScene(SCENE_OBJ);
    //SetCurrentScene(SCENE_HEIGHTMAP);
    //SetCurrentScene(SCENE_TEST);
    SetCurrentScene(0);
}


void SceneManager::Update()
{
    SAFE_UPDATE(m_pCurrentScene);
    for (auto &p : ObjectManager::Get()->GetParticles())
        p->Update();
}

void SceneManager::Render()
{
    SAFE_RENDER(m_pCurrentScene);
    for (auto &p : ObjectManager::Get()->GetParticles())
        p->Render();
}


void SceneManager::SetCurrentScene(size_t index)
{
    if (index >= m_vecScene.size()) return;

    IScene* pScene = m_vecScene[index];
    if (pScene)
    {
        pScene->Init();
        m_pCurrentScene = pScene;
    }
}

