#include "stdafx.h"
#include "SceneMapTool.h"
#include "./sample/grid/Grid.h"
#include "ImGuizmoManager.h"
#include "HeightMap.h"


SceneMapTool::SceneMapTool()
{
    m_pImGuizmoManager = NULL;
}


SceneMapTool::~SceneMapTool()
{
    OnDestructIScene();
    SAFE_DELETE(m_pImGuizmoManager);
    SAFE_RELEASE(m_pHeightMap);
}

void SceneMapTool::Init()
{
    m_pImGuizmoManager = new ImGuizmoManager(); m_pImGuizmoManager->Init();
    IDisplayObject* pObj = NULL;
    pObj = new Grid(); pObj->Init(); AddSimpleObj(pObj);
    

    D3DXMATRIXA16 matS, matT, matWorld;
    //D3DXMatrixScaling(&matS, 30.0f, 0.1f, 30.0f);
    D3DXMatrixScaling(&matS, 30.f, 10.f, 30.f);
    D3DXMatrixTranslation(&matT, 0.0f, 0.0f, 0.0f);
    matWorld = matS * matT;

    m_pHeightMap = new HeightMap; //AddSimpleObj(m_pHeightMap);
    m_pHeightMap->SetDimension(256);

    m_pHeightMap->Load(_T("./Resource/heightmap/HeightMap.raw"), &matWorld);
    m_pHeightMap->SetMtlTex(D3DMATERIAL9(DXUtil::WHITE_MTRL),
        TextureManager::Get()->GetTexture(_T("./Resource/heightmap/terrain.jpg")));
    m_pHeightMap->Init();

    //g_pMapManager->AddMap("heightmap", m_pHeightMap);
    //g_pMapManager->SetCurrentMap("heightmap");


}

void SceneMapTool::Update()
{
    OnUpdateIScene();
    SAFE_UPDATE(m_pImGuizmoManager);
    SAFE_UPDATE(m_pHeightMap);
}

void SceneMapTool::Render()
{
    OnRenderIScene();
    SAFE_RENDER(m_pImGuizmoManager);
    SAFE_RENDER(m_pHeightMap);
}

