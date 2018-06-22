#include "stdafx.h"
#include "SceneTest.h"
#include "./sample/grid/Grid.h"
#include "SampleUI.h"
#include "VertexBlending.h"
#include "TextureBlending.h"
#include "AlphaBlending.h"
#include "./Sample/MipmapFilter.h"
#include "./Sample/Picking.h"
#include "./Sample/Frustum.h"
#include "./sample/Viewport.h"
#include "./sample/shader/ColorShader.h"
#include "./sample/shader/TextureMapping.h"
#include "./sample/shader/Lighting.h"
#include "./sample/shader/LightingDiffuseShader.h"
#include "./sample/shader/ToonShader.h"
#include "./sample/shader/NormalMapping.h"


SceneTest::SceneTest()
{
	
}


SceneTest::~SceneTest()
{
	OnDestructIScene();
}

void SceneTest::Init()
{
	IDisplayObject* pObj = NULL;
	//pObj = new Grid(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new Picking(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new VertexBlending(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new SampleUI(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new TextureBlending(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new AlphaBlending(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new Frustum(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new MipmapFilter; pObj->Init(); AddSimpleObj(pObj);
	//pObj = new ViewPort(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new SkinnedMesh(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new ColorShader(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new TextureMapping(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new Lighting(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new LightingDiffuseShader(); pObj->Init(); AddSimpleObj(pObj);
	pObj = new ToonShader(); pObj->Init(); AddSimpleObj(pObj);
	//pObj = new NormalMapping(); pObj->Init(); AddSimpleObj(pObj);
	
	D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&dir, &dir);
	D3DLIGHT9 light = DXUtil::InitDirectional(&dir, &WHITE);
	DX::GetDevice()->SetLight(0, &light);
	DX::GetDevice()->LightEnable(0, true);

	DX::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	D3DXCreateCubeTextureFromFile(DX::GetDevice(), ASSET_PATH + _T("textures/cubemap.jpg"), &m_pCubemap);
}

void SceneTest::Update()
{
	OnUpdateIScene();
}

void SceneTest::Render()
{
	OnRenderIScene();
}
