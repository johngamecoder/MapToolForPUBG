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
}

void SceneMapTool::Init()
{
    m_pImGuizmoManager = new ImGuizmoManager(); m_pImGuizmoManager->Init();
    IDisplayObject* pObj = NULL;
    pObj = new Grid(); pObj->Init(); AddSimpleObj(pObj);
    
    HeightMap*	m_pHeightMap = new HeightMap(); 
    m_pHeightMap->SetDimension(257);
    D3DXMATRIXA16 matS;
    D3DXMatrixScaling(&matS, 0.2f, 0.03f, 0.2f);
    m_pHeightMap->Load(ASSET_PATH + _T("Textures/heightmap/HeightMap.raw"), &matS);
    m_pHeightMap->SetMtlTex(D3DMATERIAL9(DXUtil::WHITE_MTRL),
        TextureManager::Get()->GetTexture(ASSET_PATH + _T("Textures/heightmap/terrain.jpg")));
    pObj->Init(); AddSimpleObj(pObj);

}

void SceneMapTool::Update()
{
    OnUpdateIScene();
    SAFE_UPDATE(m_pImGuizmoManager);

}

void SceneMapTool::Render()
{
    OnRenderIScene();
    SAFE_RENDER(m_pImGuizmoManager);
}

