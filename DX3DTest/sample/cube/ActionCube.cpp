#include "StdAfx.h"
#include "ActionCube.h"

ActionCube::ActionCube()
{
	m_pVB = NULL;
	m_pIB = NULL;
}

ActionCube::~ActionCube()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

void ActionCube::Init()
{
	vector<D3DXVECTOR3> vecPos;
	for (size_t i = 0; i < g_vecCubeVertex.size(); i++)
	{
		vecPos.push_back(g_vecCubeVertex[i]);
	}

	SetVertex(m_vecVertex, m_vecIndex, vecPos);
	SetBuffer(m_pVB, m_pIB, m_vecVertex, m_vecIndex);
	
	m_pVB->GetDesc(&m_VBDesc);
	m_pIB->GetDesc(&m_IBDesc);

	auto seq = new ActionSequence();
	float radius = 8;
	for (int i = 0; i < 6; i++)
	{
		D3DXVECTOR2 pt(cos(D3DX_PI * i / 3.f) * radius, sin(D3DX_PI * i / 3.f) * radius);
		seq->AddAction(new ActionMoveTo(0.7, D3DXVECTOR3(pt.x, 0, pt.y)));
	}
	this->RunAction(new ActionRepeat(seq));
	//this->RunAction(new ActionMoveTo(2.0, D3DXVECTOR3(10, 0, 5)));
}

void ActionCube::Update()
{
	D3DXMATRIXA16 matT, matR;
	
	UpdateAction();
	LookForward(m_forward, matR);
	
	m_prevPos = m_pos;

	D3DXMatrixTranslation(&matT, m_pos.x, m_pos.y, m_pos.z);
	m_matWorld = matR * matT;
}

void ActionCube::Render()
{
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);
	DX::GetDevice()->SetTexture(0, NULL);
	DX::GetDevice()->SetFVF(VERTEX_PC::FVF);
	DX::GetDevice()->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PC));
	DX::GetDevice()->SetIndices(m_pIB);
	DX::GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		m_VBDesc.Size / sizeof(VERTEX_PC), 0, m_IBDesc.Size / sizeof(WORD) / 3);
}

void ActionCube::SetVertex(vector<VERTEX_PC> &vecOut, vector<WORD> &vecIndex, vector<D3DXVECTOR3> vecPos)
{
	D3DCOLOR red = D3DCOLOR_XRGB(255, 0, 0);
	D3DCOLOR green = D3DCOLOR_XRGB(0, 255, 0);
	D3DCOLOR blue = D3DCOLOR_XRGB(0, 0, 255);
	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);
	D3DCOLOR yellow = D3DCOLOR_XRGB(255, 255, 0);

	vecOut.push_back(VERTEX_PC(vecPos[0], white));
	vecOut.push_back(VERTEX_PC(vecPos[1], yellow));
	vecOut.push_back(VERTEX_PC(vecPos[2], green));
	vecOut.push_back(VERTEX_PC(vecPos[3], blue));
	vecOut.push_back(VERTEX_PC(vecPos[4], white));
	vecOut.push_back(VERTEX_PC(vecPos[5], yellow));
	vecOut.push_back(VERTEX_PC(vecPos[6], red));
	vecOut.push_back(VERTEX_PC(vecPos[7], green));

	for (int i = 0; i < g_vecCubeIndex.size(); i++)
	{
		vecIndex.push_back(g_vecCubeIndex[i]);
	}
}

void ActionCube::SetBuffer(LPDIRECT3DVERTEXBUFFER9 & pVb, LPDIRECT3DINDEXBUFFER9 & pIb, vector<VERTEX_PC>& vecVertex, vector<WORD>& vecIndex)
{
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

void ActionCube::LookForward(D3DXVECTOR3 & outVec3, D3DXMATRIXA16 & outMat)
{
	D3DXVECTOR3 forward = m_pos - m_prevPos;
	if (D3DXVec3LengthSq(&forward) > D3DX_16F_EPSILON)
	{
		m_forward = forward;
	}

	D3DXMatrixLookAtLH(&outMat, &D3DXVECTOR3(0, 0, 0),
		&m_forward, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&outMat, &outMat);
}
