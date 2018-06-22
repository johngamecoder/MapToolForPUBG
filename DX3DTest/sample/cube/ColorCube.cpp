#include "stdafx.h"
#include "ColorCube.h"

ColorCube::ColorCube()
{
	m_pVB = nullptr;
	m_pIB = nullptr;
	m_useIndex = false;
	m_useBuffer = false;
}


ColorCube::~ColorCube()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

void ColorCube::Init()
{
	vector<D3DXVECTOR3> vecVec3;
	for (size_t i = 0; i < g_vecCubeVertex.size(); i++)
	{
		vecVec3.push_back(g_vecCubeVertex[i]);
	}

	SetVertex(m_vecVertex, vecVec3);
	SetBuffer(m_pVB, m_vecVertex);
	
	//SetVertex(m_vecVertex, m_vecIndex, vecVec3);
	//SetBuffer(m_pVB, m_pIB, m_vecVertex, m_vecIndex);
}

void ColorCube::Update()
{
	
}

void ColorCube::Render()
{
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);
	DX::GetDevice()->SetFVF(VERTEX_PC::FVF);
	DX::GetDevice()->SetTexture(0, NULL);
	
	if (m_useIndex == false)
	{
		if (m_useBuffer == false)
		{
			DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0],
				sizeof(VERTEX_PC));
		}
		else if (m_useBuffer == true)
		{
			D3DVERTEXBUFFER_DESC vbDesc;
			m_pVB->GetDesc(&vbDesc);

			DX::GetDevice()->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PC));
			DX::GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vbDesc.Size / sizeof(VERTEX_PC) / 3);
		}
	}
	else if (m_useIndex == true)
	{
		if (m_useBuffer == false)
		{
			DX::GetDevice()->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_vecVertex.size(),
				m_vecIndex.size() / 3, &m_vecIndex[0], D3DFMT_INDEX16, &m_vecVertex[0], sizeof(VERTEX_PC));
		}
		else if (m_useBuffer == true)
		{
			D3DVERTEXBUFFER_DESC vbDesc;
			m_pVB->GetDesc(&vbDesc);
			D3DINDEXBUFFER_DESC ibDesc;
			m_pIB->GetDesc(&ibDesc);

			DX::GetDevice()->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PC));
			DX::GetDevice()->SetIndices(m_pIB);
			DX::GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
				vbDesc.Size / sizeof(VERTEX_PC), 0, ibDesc.Size / sizeof(WORD) / 3);
		}
	}
}

void ColorCube::SetVertex(vector<VERTEX_PC>& vecOut, vector<D3DXVECTOR3> vecPos)
{
	m_useIndex = false;
	WORD alpha = 255;
	D3DCOLOR red = D3DCOLOR_ARGB(alpha, 255, 0, 0);
	D3DCOLOR green = D3DCOLOR_ARGB(alpha, 0, 255, 0);
	D3DCOLOR blue = D3DCOLOR_ARGB(alpha, 0, 0, 255);
	D3DCOLOR white = D3DCOLOR_ARGB(alpha, 255, 255, 255);
	D3DCOLOR yellow = D3DCOLOR_ARGB(alpha, 255, 255, 0);
	
	vector<VERTEX_PC> vec;
	vec.push_back(VERTEX_PC(vecPos[0], white));
	vec.push_back(VERTEX_PC(vecPos[1], yellow));
	vec.push_back(VERTEX_PC(vecPos[2], green));
	vec.push_back(VERTEX_PC(vecPos[3], blue));
	vec.push_back(VERTEX_PC(vecPos[4], white));
	vec.push_back(VERTEX_PC(vecPos[5], yellow));
	vec.push_back(VERTEX_PC(vecPos[6], red));
	vec.push_back(VERTEX_PC(vecPos[7], green));

	for (int i = 0; i < g_vecCubeIndex.size(); i++)
	{
		vecOut.push_back(vec[g_vecCubeIndex[i]]);
	}
}

void ColorCube::SetVertex(vector<VERTEX_PC>& vecOut, vector<WORD> &vecIndex, vector<D3DXVECTOR3> vecPos)
{
	m_useIndex = true;

	D3DCOLOR red = D3DCOLOR_XRGB(255, 0, 0);
	D3DCOLOR green = D3DCOLOR_XRGB(0, 255, 0);
	D3DCOLOR blue = D3DCOLOR_XRGB(0, 0, 255);

	vecOut.push_back(VERTEX_PC(vecPos[0], blue));
	vecOut.push_back(VERTEX_PC(vecPos[1], red));
	vecOut.push_back(VERTEX_PC(vecPos[2], green));
	vecOut.push_back(VERTEX_PC(vecPos[3], red));
	vecOut.push_back(VERTEX_PC(vecPos[4], blue));
	vecOut.push_back(VERTEX_PC(vecPos[5], red));
	vecOut.push_back(VERTEX_PC(vecPos[6], green));
	vecOut.push_back(VERTEX_PC(vecPos[7], red));

	for (int i = 0; i < g_vecCubeIndex.size(); i++)
	{
		vecIndex.push_back(g_vecCubeIndex[i]);
	}
}

void ColorCube::SetBuffer(LPDIRECT3DVERTEXBUFFER9 &pVb, vector<VERTEX_PC> &vecVertex)
{
	m_useBuffer = true;

	DX::GetDevice()->CreateVertexBuffer(vecVertex.size() * sizeof(VERTEX_PC),
		NULL, VERTEX_PC::FVF, D3DPOOL_MANAGED, &pVb, NULL);

	VERTEX_PC* pVertex;
	pVb->Lock(0, 0, (LPVOID*)&pVertex, 0);
	memcpy(pVertex, &vecVertex[0], vecVertex.size() * sizeof(VERTEX_PC));
	/*for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		pVertex[i] = vecVertex[i];
	}*/
	pVb->Unlock();
	vecVertex.clear();
}

void ColorCube::SetBuffer(LPDIRECT3DVERTEXBUFFER9 & pVb, LPDIRECT3DINDEXBUFFER9 & pIb,
	vector<VERTEX_PC>& vecVertex, vector<WORD>& vecIndex)
{
	m_useBuffer = true;

	DX::GetDevice()->CreateVertexBuffer(vecVertex.size() * sizeof(VERTEX_PC),
		NULL, VERTEX_PC::FVF, D3DPOOL_MANAGED, &pVb, NULL);
	
	VERTEX_PC* pVertex;
	pVb->Lock(0, 0, (LPVOID*)&pVertex, 0);
	memcpy(pVertex, &vecVertex[0], vecVertex.size() * sizeof(VERTEX_PC));
	/*for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		pVertex[i] = vecVertex[i];
	}*/
	pVb->Unlock();
	vecVertex.clear();


	DX::GetDevice()->CreateIndexBuffer(vecIndex.size() * sizeof(WORD),
		NULL, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIb, NULL);

	WORD* pIndex;
	pIb->Lock(0, 0, (LPVOID*)&pIndex, 0);
	memcpy(pIndex, &vecIndex[0], vecIndex.size() * sizeof(WORD));
	/*for (size_t i = 0; i < vecIndex.size(); ++i)
	{
		pIndex[i] = vecIndex[i];
	}*/
	pIb->Unlock();
	vecIndex.clear();
}
