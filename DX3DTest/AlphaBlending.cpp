#include "stdafx.h"
#include "AlphaBlending.h"


AlphaBlending::AlphaBlending()
{
}


AlphaBlending::~AlphaBlending()
{
}

void AlphaBlending::Init()
{
	D3DCOLOR c = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-1, -1, 0), c));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-1, 1, 0), c));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(1, 1, 0), c));

	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-1, -1, 0), c));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(1, 1, 0), c));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(1, -1, 0), c));
	

	c = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.3f);
	m_vecVertex1.push_back(VERTEX_PC(D3DXVECTOR3(-1, -1, 0), c));
	m_vecVertex1.push_back(VERTEX_PC(D3DXVECTOR3(-1, 1, 0), c));
	m_vecVertex1.push_back(VERTEX_PC(D3DXVECTOR3(1, 1, 0), c));
	
	m_vecVertex1.push_back(VERTEX_PC(D3DXVECTOR3(-1, -1, 0), c));
	m_vecVertex1.push_back(VERTEX_PC(D3DXVECTOR3(1, 1, 0), c));
	m_vecVertex1.push_back(VERTEX_PC(D3DXVECTOR3(1, -1, 0), c));
	
	D3DXMATRIXA16 mat1;
	D3DXMatrixTranslation(&mat1, 1, 1, -0.4f);
	for (auto& p : m_vecVertex1)
	{
		D3DXVec3TransformCoord(&p.p, &p.p, &mat1);
	}
}

void AlphaBlending::Update()
{
	D3DXMATRIXA16 mat;
	DX::GetDevice()->GetTransform(D3DTS_VIEW, &mat);
	m_matWorld._11 = mat._11;
	m_matWorld._13 = mat._13;
	m_matWorld._31 = mat._31;
	m_matWorld._31 = mat._31;

	m_matWorld = mat;
	m_matWorld._41 = 0.0f;
	m_matWorld._42 = 0.0f;
	m_matWorld._43 = 0.0f;
	D3DXMatrixInverse(&m_matWorld, NULL, &m_matWorld);
}

void AlphaBlending::Render()
{
	//DX::GetDevice()->SetRenderState(D3DRS_ZENABLE, false);
	//DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//D3DBLENDOP_SUBTRACT D3DBLENDOP_ADD
	DX::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
	DX::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//D3DBLEND_ONE D3DBLEND_SRCALPHA  D3DBLEND_INVSRCALPHA D3DBLEND_SRCCOLOR D3DBLEND_INVSRCCOLOR
	//D3DBLEND_ZERO
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	DX::GetDevice()->SetFVF(VERTEX_PC::FVF);
	DX::GetDevice()->SetTexture(0, NULL);
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);
	DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(VERTEX_PC));
	DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex1.size() / 3, &m_vecVertex1[0], sizeof(VERTEX_PC));

	DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DX::GetDevice()->SetRenderState(D3DRS_ZENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
