#include "stdafx.h"
#include "Lighting.h"


Lighting::Lighting()
{
	
}


Lighting::~Lighting()
{
	SAFE_RELEASE(m_pMesh);
}


void Lighting::Init()
{
	//쉐이더 지정
	m_pEffect = ShaderManager::Get()->GetShader(SHADER_PATH + _T("Lighting.fx"));

	//모델 지정
	LPCTSTR xPath = ASSET_PATH + _T("Models/zealot/zealot.x");
	//LPCTSTR xPath = ASSET_PATH + _T("Models/sphere.x");
	if (FAILED(D3DXLoadMeshFromX(xPath, D3DXMESH_MANAGED, DX::GetDevice(), NULL, NULL, NULL, NULL, &m_pMesh)))
		assert(false && "Loading x file is failed");
	
	
	//매트릭스 업데이트
	m_pEffect->SetMatrix("gProjMatrix", Camera::Get()->GetProjMatrix());
	
	//텍스처 지정
	LPDIRECT3DTEXTURE9 pTex = NULL;
	pTex = TextureManager::Get()->GetTexture(ASSET_PATH + _T("Models/zealot/Zealot_Diffuse.bmp"));
	m_pEffect->SetTexture("DiffuseMap_Tex", pTex);
	//m_pEffect->CommitChanges();
}

void Lighting::Update()
{
	//광원 위치 업데이트
	static float angle = -D3DX_PI / 6;
	//angle += 0.02f;
	float radius = 500;
	D3DXVECTOR4 lightPos(cosf(angle) * radius, radius, sinf(angle) * radius, 1);
	m_pEffect->SetVector("gWorldLightPosition", &lightPos);
}

void Lighting::Render()
{
	//카메라 위치 업데이트
	D3DXVECTOR4 camPos(Camera::Get()->GetPosition(), 1);
	m_pEffect->SetVector("gWorldCameraPosition", &camPos);

	//매트릭스 업데이트
	m_pEffect->SetMatrix("gWorldMatrix", &m_matWorld);
	m_pEffect->SetMatrix("gViewMatrix", Camera::Get()->GetViewMatrix());

	LPDIRECT3DVERTEXSHADER9 vs;
	LPDIRECT3DPIXELSHADER9 ps;
	D3DVERTEXELEMENT9 vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };
	//D3DXDeclaratorFromFVF(m_pMesh->GetFVF(), vertexDecl);
	D3DXDeclaratorFromFVF(VERTEX_PC::FVF, vertexDecl);

	LPDIRECT3DVERTEXDECLARATION9    pFVF = NULL;
	DX::GetDevice()->CreateVertexDeclaration(vertexDecl, &pFVF);
	DX::GetDevice()->SetVertexDeclaration(pFVF);
	
	m_pEffect->SetTechnique("Lighting");
	
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);
	
	for (int i = 0; i < numPasses; i++)
	{
		m_pEffect->BeginPass(i);
		
		m_pMesh->DrawSubset(0);
		
		m_pEffect->EndPass();
	}

	m_pEffect->End();
	SAFE_RELEASE(pFVF);
	//DX::GetDevice()->SetVertexDeclaration(NULL);
	//DX::GetDevice()->SetVertexShader(NULL);
	//DX::GetDevice()->SetPixelShader(NULL);
	
}
