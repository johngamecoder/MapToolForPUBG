#include "stdafx.h"
#include "SceneHeightmap.h"
#include "HeightMap.h"
#include "./model/ase/AseUnit.h"

SceneHeightmap::SceneHeightmap()
{
	m_pHeightMap = NULL;
	m_pAseUnit = NULL;
}


SceneHeightmap::~SceneHeightmap()
{
	OnDestructIScene();
}

void SceneHeightmap::Init()
{
	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 0.2f, 0.03f, 0.2f);

	m_pHeightMap = new HeightMap; AddSimpleObj(m_pHeightMap);
	m_pHeightMap->SetDimension(257);
	
	m_pHeightMap->Load(ASSET_PATH + _T("Textures/heightmap/HeightMap.raw"), &matS);
	m_pHeightMap->SetMtlTex(D3DMATERIAL9(DXUtil::WHITE_MTRL),
		TextureManager::Get()->GetTexture(ASSET_PATH + _T("Textures/heightmap/terrain.jpg")));
	m_pHeightMap->Init();

	g_pMapManager->AddMap("heightmap", m_pHeightMap);
	g_pMapManager->SetCurrentMap("heightmap");
	

	m_pAseUnit = new AseUnit(); m_pAseUnit->Init(); AddSimpleObj(m_pAseUnit);
	

	D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&dir, &dir);
	D3DLIGHT9 light = DXUtil::InitDirectional(&dir, &WHITE);
	DX::GetDevice()->SetLight(0, &light);
	DX::GetDevice()->LightEnable(0, true);

	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	DX::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, true);

}


void SceneHeightmap::Update()
{
	OnUpdateIScene();
}

void SceneHeightmap::Render()
{
	OnRenderIScene();
}

