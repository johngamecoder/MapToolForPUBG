#include "stdafx.h"
#include "TextureMapping.h"

TextureMapping::TextureMapping()
{
}

TextureMapping::~TextureMapping()
{
	SAFE_RELEASE(m_pMesh);
}


void TextureMapping::Init()
{
	//쉐이더 지정
	m_pEffect = ShaderManager::Get()->GetShader(SHADER_PATH + _T("TextureMapping.fx"));

	//모델 지정
	LPCTSTR xPath = ASSET_PATH + _T("Models/sphere.x");
	//LPCTSTR xPath = ASSET_PATH + _T("Models/zealot/zealot.x");
	if (FAILED(D3DXLoadMeshFromX(xPath, D3DXMESH_MANAGED, DX::GetDevice(), NULL, NULL, NULL, NULL, &m_pMesh)))
		assert(false && "Loading x file is failed");

	//텍스처 지정
	m_pTex = TextureManager::Get()->GetTexture(ASSET_PATH + _T("Textures/earth.jpg"));
	m_pEffect->SetTexture("DiffuseMap_Tex", m_pTex);

	//매트릭스 업데이트
	m_pEffect->SetMatrix("gProjMatrix", Camera::Get()->GetProjMatrix());

	D3DXVECTOR4 color(1, 0, 0, 1);
	m_pEffect->SetVector("gOutlineColor", &color);
	m_pEffect->SetFloat("gConst", 1.0f);
}

void TextureMapping::Update()
{
	static float angle = 0;
	angle += 0.02f;
	float radius = 4;

	m_pEffect->SetFloat("gCos", cos(angle));
	
	//매트릭스 업데이트
	D3DXMATRIXA16 matS, matR, matT;
	D3DXMatrixTranslation(&matT, cosf(angle) * radius, 0, sinf(angle) * radius);
	//D3DXMatrixIdentity(&matT);
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
	float axis = 23.5f / 180 * D3DX_PI;
	D3DXMatrixRotationAxis(&matR, &D3DXVECTOR3(sin(axis), cos(axis), 0), angle);
	m_matWorld = matS  * matT;
	m_pEffect->SetMatrix("gWorldMatrix", &m_matWorld);
	m_pEffect->SetMatrix("gViewMatrix", Camera::Get()->GetViewMatrix());
}

void TextureMapping::Render()
{
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);

	for (int i = 0; i < numPasses; i++)
	{
		if ( i == 0 )
			DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		else if ( i == 1)
			DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pEffect->BeginPass(i);
		m_pMesh->DrawSubset(0);
		m_pEffect->EndPass();
	}

	m_pEffect->End();
}
