#include "stdafx.h"
#include "LightingDiffuseShader.h"


LightingDiffuseShader::LightingDiffuseShader()
{
	
}


LightingDiffuseShader::~LightingDiffuseShader()
{
	SAFE_RELEASE(m_pMesh);
}


void LightingDiffuseShader::Init()
{
	//쉐이더 지정
	m_pEffect = ShaderManager::Get()->GetShader(SHADER_PATH + _T("LightingDiffuse.fx"));

	//모델 지정
	//LPCTSTR xPath = ASSET_PATH + _T("Models/zealot/zealot.x");
	LPCTSTR xPath = ASSET_PATH + _T("Models/sphere.x");
	if (FAILED(D3DXLoadMeshFromX(xPath, D3DXMESH_MANAGED, DX::GetDevice(), NULL, NULL, NULL, NULL, &m_pMesh)))
		assert(false && "Loading x file is failed");

	D3DXVECTOR4 color(1, 1, 1, 0);
	//m_pEffect->SetVector("gLightColor", &color);

	//매트릭스 업데이트
	m_pEffect->SetMatrix("gProjMatrix", Camera::Get()->GetProjMatrix());
	
	//텍스처 지정
	LPDIRECT3DTEXTURE9 pTex = NULL;
	pTex = TextureManager::Get()->GetTexture(ASSET_PATH + _T("Textures/fieldStone/Fieldstone_DM.tga"));
	m_pEffect->SetTexture("DiffuseMap_Tex", pTex);
	pTex = TextureManager::Get()->GetTexture(ASSET_PATH + _T("Textures/fieldStone/Fieldstone_SM.tga"));
	m_pEffect->SetTexture("SpecularMap_Tex", pTex);
}

void LightingDiffuseShader::Update()
{
	//카메라 위치 업데이트
	D3DXVECTOR4 camPos(Camera::Get()->GetPosition(), 1);
	m_pEffect->SetVector("gWorldCameraPosition", &camPos);
	
	//광원 위치 업데이트
	static float angle = -D3DX_PI / 6;
	//angle += 0.02f;
	float radius = 500;
	D3DXVECTOR4 lightPos(cosf(angle) * radius, radius, sinf(angle) * radius, 1);
	m_pEffect->SetVector("gWorldLightPosition", &lightPos);

	//매트릭스 업데이트
	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
	m_matWorld = matS;

	m_pEffect->SetMatrix("gWorldMatrix", &m_matWorld);
	m_pEffect->SetMatrix("gViewMatrix", Camera::Get()->GetViewMatrix());
}

void LightingDiffuseShader::Render()
{
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);

	for (int i = 0; i < numPasses; i++)
	{
		m_pEffect->BeginPass(i);
		m_pMesh->DrawSubset(0);
		m_pEffect->EndPass();
	}

	m_pEffect->End();
}
