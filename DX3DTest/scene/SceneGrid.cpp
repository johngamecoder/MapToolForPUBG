#include "stdafx.h"
#include "SceneGrid.h"
#include "./sample/grid/Grid.h"
#include "./sample/grid/Walls.h"
#include "./sample/grid/Hexagon.h"
#include "./sample/cube/ColorCube.h"
#include "./sample/cube/CubeMan.h"
#include "./sample/cube/ActionCube.h"

SceneGrid::SceneGrid()
{
	
}


SceneGrid::~SceneGrid()
{
	OnDestructIScene();
}

void SceneGrid::Init()
{
	IDisplayObject* pObj = NULL;
	pObj = new Grid(); pObj->Init(); AddSimpleObj(pObj);
	pObj = new ColorCube(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new Cubeman(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new Walls(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new Hexagon(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new ActionCube(); pObj->Init(); AddSimpleObj(pObj);

	SetLight();
	
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	DX::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	//DX::GetDevice()->SetRenderState(D3DRS_SPECULARENABLE, true);
	//DX::GetDevice()->SetRenderState(D3DRS_AMBIENT, 0x00804040);
}

void SceneGrid::Update()
{
	//SetLight();
	OnUpdateIScene();
}

void SceneGrid::Render()
{
	if (GetAsyncKeyState('L') & 0x0001)
	{
		static bool b = true;
		b = !b;
		DX::GetDevice()->LightEnable(0, b);
	}
	OnRenderIScene();
}


void SceneGrid::SetLight()
{
	static float angle = D3DX_PI / 4;
	angle += 0.04f;
	
	D3DXVECTOR3 dir(cos(angle), -1, sin(angle));
	D3DXVec3Normalize(&dir, &dir);
	D3DLIGHT9 light = DXUtil::InitDirectional(&dir, &D3DXCOLOR(0.85f, 0.85f, 0.7f, 0.8f));
	
	DX::GetDevice()->SetLight(0, &light);
	DX::GetDevice()->LightEnable(0, true);
}
