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
    
    ////------------------HeightMap------------------
    ////old map
    //D3DXMATRIXA16 matS, matT, matWorld;
    //D3DXMatrixScaling(&matS, 30.0f, 1.0f, 30.0f);
    //D3DXMatrixTranslation(&matT, 0.0f, 0.0f, 0.0f);
    //matWorld = matS * matT;

    //m_pHeightMap = new HeightMap;
    //m_pHeightMap->SetDimension(257);
    //m_pHeightMap->Load(_T("./Resource/heightmap/HeightMap.raw"), &matWorld);
    //m_pHeightMap->SetMtlTex(D3DMATERIAL9(DXUtil::WHITE_MTRL),
    //    TextureManager::Get()->GetTexture(_T("./Resource/heightmap/terrain.jpg")));


    ////---------------------------------------------------------


    //New map
    D3DXMATRIXA16 matS, matT, matWorld;
    D3DXMatrixScaling(&matS, 100.0f, 50.0f, 100.0f);
    D3DXMatrixTranslation(&matT, 0.0f, 0.0f, 0.0f);
    matWorld = matS * matT;

    //m_pHeightMap = new HeightMap;
    //m_pHeightMap->SetDimension(256);
    //m_pHeightMap->Load(_T("./Resource/heightmap/testing/HeightMap.raw"), &matWorld);
    //m_pHeightMap->SetMtlTex(D3DMATERIAL9(DXUtil::WHITE_MTRL),
    //    TextureManager::Get()->GetTexture(_T("./Resource/heightmap/terrain.jpg")));

    m_pHeightMap = new HeightMap;
    m_pHeightMap->SetDimension(256);
    m_pHeightMap->Load(_T("./Resource/heightmap/testing/HeightMap_128.raw"), &matWorld);
    m_pHeightMap->SetMtlTex(D3DMATERIAL9(DXUtil::WHITE_MTRL),
        TextureManager::Get()->GetTexture(_T("./Resource/heightmap/ground_d_8192.png"/*testing/HeightMap.png"*/)));


    /*
    m_pHeightMap->SetDimension(256);

    
    m_pHeightMap->Load(_T("./Resource/heightmap/testing/HeightMap.raw"), &matWorld);
    m_pHeightMap->SetMtlTex(D3DMATERIAL9(DXUtil::WHITE_MTRL),
        TextureManager::Get()->GetTexture(_T("./Resource/heightmap/testing/Plain_BaseColor.tga")));
    */





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

