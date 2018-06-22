#pragma once

class Pyramid : public IObject
{
private:
	D3DXMATRIXA16		m_matWorld;
	vector<VERTEX_PC>	m_vecVertex;
	D3DCOLOR			m_color;

public:
	
	Pyramid(D3DCOLOR c, D3DXMATRIXA16& matWorld);
	~Pyramid();
	void Init();
	void Render();
};

