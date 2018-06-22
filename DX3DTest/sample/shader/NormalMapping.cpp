#include "stdafx.h"
#include "NormalMapping.h"

NormalMapping::~NormalMapping()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pCubeTex);
}

void NormalMapping::Init()
{
	//쉐이더 지정
	//m_pEffect = ShaderManager::Get()->GetShader(SHADER_PATH + _T("NormalMapping.fx"));
	m_pEffect = ShaderManager::Get()->GetShader(SHADER_PATH + _T("EnvMapping.fx"));

	//모델 지정
	LPCTSTR xPath = ASSET_PATH + _T("Models/TeapotWithTangent.x");
	//LPCTSTR xPath = ASSET_PATH + _T("Models/sphere.x");
	//LPCTSTR xPath = ASSET_PATH + _T("Models/SphereWithTangent.x");
	if (FAILED(D3DXLoadMeshFromX(xPath, D3DXMESH_MANAGED, DX::GetDevice(), NULL, NULL, NULL, NULL, &m_pMesh)))
		assert(false && "Loading x file is failed");

	D3DXVECTOR4 color(1, 1, 1, 0);
	m_pEffect->SetVector("gLightColor", &color);

	//텍스처 지정
	LPDIRECT3DTEXTURE9 pTex = NULL;
	pTex = TextureManager::Get()->GetTexture(ASSET_PATH + _T("Textures/fieldStone/Fieldstone_DM.tga"));
	m_pEffect->SetTexture("DiffuseMap_Tex", pTex);
	pTex = TextureManager::Get()->GetTexture(ASSET_PATH + _T("Textures/fieldStone/Fieldstone_SM.tga"));
	m_pEffect->SetTexture("SpecularMap_Tex", pTex);
	pTex = TextureManager::Get()->GetTexture(ASSET_PATH + _T("Textures/fieldStone/Fieldstone_NM.tga"));
	m_pEffect->SetTexture("NormalMap_Tex", pTex);
	
	D3DXCreateCubeTextureFromFile(DX::GetDevice(), ASSET_PATH + _T("Textures/snow_env.dds"), &m_pCubeTex);
	m_pEffect->SetTexture("EnvMap_Tex", m_pCubeTex);
	
}

void NormalMapping::Update()
{
	//광원 위치 업데이트
	static float angle = -D3DX_PI / 6;
	//angle += 0.02f;
	float radius = 500;
	D3DXVECTOR4 lightPos(cosf(angle) * radius, radius, sinf(angle) * radius, 1);
	m_pEffect->SetVector("gWorldLightPosition", &lightPos);

	//카메라 위치 업데이트
	D3DXVECTOR4 camPos(Camera::Get()->GetPosition(), 1);
	m_pEffect->SetVector("gWorldCameraPosition", &camPos);

	//매트릭스 업데이트
	D3DXMATRIXA16 matS, matVP;
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
	m_matWorld = matS;
	m_pEffect->SetMatrix("gWorldMatrix", &m_matWorld);
	matVP = *Camera::Get()->GetViewProjMatrix();
	m_pEffect->SetMatrix("gWVPMatrix", &(m_matWorld * matVP));
}

void NormalMapping::Render()
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
