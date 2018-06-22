#include "stdafx.h"
#include "Hexagon.h"


Hexagon::Hexagon(WORD radius)
{
	m_radius = radius;
}


Hexagon::~Hexagon()
{
}

void Hexagon::Init()
{
	D3DCOLOR color = YELLOW;
	for (int i = 0; i < 6; i++)
	{
		D3DXVECTOR2 pt(cos(D3DX_PI * i / 3.f) * m_radius, sin(D3DX_PI * i / 3.f) * m_radius);
		m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(pt.x, 0, pt.y), color));
	}
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(m_radius, 0, 0), color));
}

void Hexagon::Update()
{
}

void Hexagon::Render()
{
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	DX::GetDevice()->SetTexture(0, NULL);
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);
	DX::GetDevice()->SetFVF(VERTEX_PC::FVF);
	DX::GetDevice()->DrawPrimitiveUP(D3DPT_LINESTRIP, m_vecVertex.size() - 1,
		&m_vecVertex[0], sizeof(VERTEX_PC));
}
