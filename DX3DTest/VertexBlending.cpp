#include "stdafx.h"
#include "VertexBlending.h"


VertexBlending::VertexBlending()
{
}


VertexBlending::~VertexBlending()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pTex);
}

void VertexBlending::Init()
{
	m_numVertices = 100;
	int numHalfVertices = m_numVertices / 2;
	float weights1[3] = { 0.5, 0.5, 0 };
	float weights2[3] = { 0.5, 0.5, 0 };

	DX::GetDevice()->CreateVertexBuffer(m_numVertices * sizeof(VERTEX_BLENDING), 0,
		VERTEX_BLENDING::FVF, D3DPOOL_DEFAULT, &m_pVB, 0);

	VERTEX_BLENDING* pV;
	m_pVB->Lock(0, 0, (LPVOID*)&pV, 0);
	
	for (int i = 0; i < numHalfVertices; i++)
	{
		float theta = (2 * D3DX_PI * i) / (float)(numHalfVertices - 1);
		D3DXCOLOR color(0.015f * i, 0.015f * i, 0.015f * i, 1.0f);
		//color = (WHITE);
		
		pV[2 * i + 0] = VERTEX_BLENDING(D3DXVECTOR3(cos(theta), 1.0f, sin(theta))
			, weights1, 0x00000100, color, D3DXVECTOR2((i) / (float)(numHalfVertices - 1), 0));
		
		pV[2 * i + 1] = VERTEX_BLENDING(D3DXVECTOR3(cos(theta), -1.0f, sin(theta))
			, weights2, 0x00000200, color, D3DXVECTOR2((i) / (float)(numHalfVertices - 1), 1));
	}
	
	m_pVB->Unlock();

	m_pTex = TextureManager::Get()->GetTexture(ASSET_PATH + _T("textures/ham1.png"));
}

void VertexBlending::Update()
{
	D3DXMatrixIdentity(&m_mat0);
	DWORD d = GetTickCount() % ((int)((D3DX_PI * 2 * 1000)));
	D3DXMatrixRotationY(&m_mat1, d / 1000.0f);
	D3DXMatrixScaling(&m_mat2, 0.5 * sinf(d / 1000.0f), 0, 0.5 * sinf(d / 1000.0f));
	D3DXMatrixTranslation(&m_mat3, 0, 2 * sinf(d / 1000.0f), 0);
}

void VertexBlending::Render()
{
	//pre
	DX::GetDevice()->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS);
	DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	DX::GetDevice()->SetTransform(D3DTS_WORLDMATRIX(0), &m_mat0);
	DX::GetDevice()->SetTransform(D3DTS_WORLDMATRIX(1), &m_mat1);
	DX::GetDevice()->SetTransform(D3DTS_WORLDMATRIX(2), &m_mat2);
	DX::GetDevice()->SetTransform(D3DTS_WORLDMATRIX(3), &m_mat3);
	DX::GetDevice()->SetTexture(0, m_pTex);
	
	DX::GetDevice()->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_BLENDING));
	DX::GetDevice()->SetFVF(VERTEX_BLENDING::FVF);
	DX::GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_numVertices - 2);
	
	//post
	DX::GetDevice()->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, false);
	DX::GetDevice()->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
