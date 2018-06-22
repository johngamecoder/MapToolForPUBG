#include "StdAfx.h"
#include "Pyramid.h"


Pyramid::Pyramid(D3DCOLOR c, D3DXMATRIXA16& matWorld)
{
	m_color = c;
	m_matWorld = matWorld;
}


Pyramid::~Pyramid()
{
}

void Pyramid::Init()
{
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(0, 0, 0), m_color));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(1, -1, -1), m_color));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-1, -1, -1), m_color));

	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(0, 0, 0), m_color));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(1, -1, 1), m_color));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(1, -1, -1), m_color));

	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(0, 0, 0), m_color));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-1, -1, 1), m_color));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(1, -1, 1), m_color));

	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(0, 0, 0), m_color));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-1, -1, -1), m_color));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-1, -1, 1), m_color));

	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(1, -1, -1), m_color));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-1, -1, 1), m_color));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-1, -1, -1), m_color));

	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(1, -1, -1), m_color));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(1, -1, 1), m_color));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-1, -1, 1), m_color));
}

void Pyramid::Render()
{
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);
	DX::GetDevice()->SetFVF(VERTEX_PC::FVF);
	DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 
		m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(VERTEX_PC));
}
