#include "stdafx.h"
#include "SceneObjMap.h"
#include "./model/obj/ObjMap.h"
#include "./sample/cube/Cubeman.h"

SceneObjMap::SceneObjMap()
{
	m_pObjMap = NULL;
	m_pCubeman = NULL;
}


SceneObjMap::~SceneObjMap()
{
	OnDestructIScene();
}


void SceneObjMap::Init()
{
	m_pObjMap = new ObjMap(); m_pObjMap->Init(); AddSimpleObj((m_pObjMap));
	m_pCubeman = new Cubeman(); m_pCubeman->Init(); AddSimpleObj(m_pCubeman);
	
	D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&dir, &dir);
	D3DLIGHT9 light = DXUtil::InitDirectional(&dir, &WHITE);
	DX::GetDevice()->SetLight(0, &light);
	DX::GetDevice()->LightEnable(0, true);

	DX::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, true);
}


void SceneObjMap::Update()
{
	OnUpdateIScene();
}

void SceneObjMap::Render()
{
	OnRenderIScene();
}


