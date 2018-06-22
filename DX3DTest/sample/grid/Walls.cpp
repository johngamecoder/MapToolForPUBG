#include "stdafx.h"
#include "Walls.h"


Walls::Walls()
{
}


Walls::~Walls()
{
}

void Walls::Init()
{
	vector<D3DXVECTOR3> vecWall;
	vecWall.push_back(D3DXVECTOR3(-1.0f, -1.0f, -1.0f));
	vecWall.push_back(D3DXVECTOR3(-1.0f, 1.0f, -1.0f));
	vecWall.push_back(D3DXVECTOR3(1.0f, 1.0f, -1.0f));
	vecWall.push_back(D3DXVECTOR3(-1.0f, -1.0f, -1.0f));
	vecWall.push_back(D3DXVECTOR3(1.0f, 1.0f, -1.0f));
	vecWall.push_back(D3DXVECTOR3(1.0f, -1.0f, -1.0f));
	
	D3DXMATRIXA16 matS, matT;
	//Áß¾Ó º®
	MakePlaneGroup(m_vecWall0, vecWall);
	D3DXMatrixTranslation(&matT, 0, 0, 5);
	for (size_t i = 0; i < m_vecWall0.size(); i++)
	{
		D3DXVec3TransformCoord(&m_vecWall0[i].p, &m_vecWall0[i].p, &matT);
	}
	for (auto p : m_vecWall0)
		m_vecVertex.push_back(p);

	//¿ìÃø º®
	MakePlaneGroup(m_vecWall1, vecWall);
	D3DXMatrixTranslation(&matT, 5, 0, 5);
	for (size_t i = 0; i < m_vecWall0.size(); i++)
	{
		D3DXVec3TransformCoord(&m_vecWall1[i].p, &m_vecWall1[i].p, &matT);
	}
	for (auto p : m_vecWall1)
		m_vecVertex.push_back(p);

	//ÁÂÃø º®
	MakePlaneGroup(m_vecWall2, vecWall);
	D3DXMatrixTranslation(&matT, -5, 0, 5);
	for (size_t i = 0; i < m_vecWall0.size(); i++)
	{
		D3DXVec3TransformCoord(&m_vecWall2[i].p, &m_vecWall2[i].p, &matT);
	}
	for (auto p : m_vecWall2)
		m_vecVertex.push_back(p);
	
	D3DXVECTOR3 pos(5.0f, 0.0f, 4.0f);
	D3DLIGHT9 light = DXUtil::InitPoint(&pos, &RED);
	light.Range = 2.4f;
	DX::GetDevice()->SetLight(3, &light);
	DX::GetDevice()->LightEnable(3, true);
	
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	pos = D3DXVECTOR3(-5.0f, 2.0f, -2.0f);
	D3DXVECTOR3 dir(0.0f, 0.0f, 1.0f);
	D3DXVec3Normalize(&dir, &dir);
	light = DXUtil::InitSpot(&dir, &pos, &BLUE);
	light.Range = 25.0f;
	light.Theta = D3DX_PI / 6;
	light.Phi = D3DX_PI / 3;
	light.Falloff = 3;
	DX::GetDevice()->SetLight(2, &light);
	DX::GetDevice()->LightEnable(2, true);
	

	vector<VERTEX_PN> vecFloor;
	vecFloor.push_back(VERTEX_PN(D3DXVECTOR3(-1.0f, 0.0f, -1.0f), D3DXVECTOR3(-1, 0, -1)));
	vecFloor.push_back(VERTEX_PN(D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXVECTOR3(-1, 0, 1)));
	vecFloor.push_back(VERTEX_PN(D3DXVECTOR3(1.0f, 0.0f, 1.0f), D3DXVECTOR3(1, 0, 1)));
	vecFloor.push_back(VERTEX_PN(D3DXVECTOR3(1.0f, 0.0f, -1.0f), D3DXVECTOR3(1, 0, -1)));
	
	m_vecFloor0.push_back(vecFloor[0]);
	m_vecFloor0.push_back(vecFloor[1]);
	m_vecFloor0.push_back(vecFloor[2]);
	m_vecFloor0.push_back(vecFloor[0]);
	m_vecFloor0.push_back(vecFloor[2]);
	m_vecFloor0.push_back(vecFloor[3]);
	
	D3DXMatrixScaling(&matS, 4, 4, 4);
	D3DXMatrixTranslation(&matT, -5, 0, -10);
	for (size_t i = 0; i < m_vecFloor0.size(); i++)
	{
		D3DXVec3TransformCoord(&m_vecFloor0[i].p, &m_vecFloor0[i].p, &(matS *matT));
	}
	for (auto p : m_vecFloor0)
		m_vecVertex.push_back(p);


	vecFloor.clear();
	vecFloor.push_back(VERTEX_PN(D3DXVECTOR3(-1.0f, 0.0f, -1.0f), D3DXVECTOR3(0, 1, 0)));
	vecFloor.push_back(VERTEX_PN(D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXVECTOR3(0, 1, 0)));
	vecFloor.push_back(VERTEX_PN(D3DXVECTOR3(1.0f, 0.0f, 1.0f), D3DXVECTOR3(0, 1, 0)));
	vecFloor.push_back(VERTEX_PN(D3DXVECTOR3(1.0f, 0.0f, -1.0f), D3DXVECTOR3(0, 1, 0)));
	
	m_vecFloor1.push_back(vecFloor[0]);
	m_vecFloor1.push_back(vecFloor[1]);
	m_vecFloor1.push_back(vecFloor[2]);
	m_vecFloor1.push_back(vecFloor[0]);
	m_vecFloor1.push_back(vecFloor[2]);
	m_vecFloor1.push_back(vecFloor[3]);

	D3DXMatrixScaling(&matS, 4, 4, 4);
	D3DXMatrixTranslation(&matT, 5, 0, -10);
	for (size_t i = 0; i < m_vecFloor1.size(); i++)
	{
		D3DXVec3TransformCoord(&m_vecFloor1[i].p, &m_vecFloor1[i].p, &(matS *matT));
	}
	for (auto p : m_vecFloor1)
		m_vecVertex.push_back(p);
}

void Walls::Update()
{
}

void Walls::Render()
{
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);
	DX::GetDevice()->SetFVF(VERTEX_PN::FVF);
	DX::GetDevice()->SetMaterial(&DXUtil::WHITE_MTRL);
	DX::GetDevice()->SetTexture(0, NULL);
	DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3,
		&m_vecVertex[0], sizeof(VERTEX_PN));
}

void Walls::MakePlane(vector<VERTEX_PN>& vecOut, vector<D3DXVECTOR3> &vecPos, D3DXMATRIXA16 &mat)
{
	D3DXVECTOR3 tmpVec3;
	D3DXVECTOR3 normal;
	
	for (size_t i = 0; i < vecPos.size(); i++)
	{
		D3DXVec3TransformCoord(&tmpVec3, &vecPos[i], &mat);

		if (i % 6 == 0)
			DXUtil::ComputeNormal(&normal, &vecPos[0], &vecPos[1], &vecPos[2]);

		vecOut.push_back(VERTEX_PN(tmpVec3, normal));
	}
}

void Walls::MakePlaneGroup(vector<VERTEX_PN>& vecOut, vector<D3DXVECTOR3> vecWall)
{
	D3DXMATRIXA16 matS, matR, matT, mat;
	float size = 1.5;
	D3DXMatrixScaling(&matS, size, size, size);
	D3DXMatrixTranslation(&matT, 0, size, 0);
	mat = matS * matT;
	MakePlane(vecOut, vecWall, mat);

	D3DXMatrixScaling(&matS, size, size, size);
	D3DXMatrixRotationY(&matR, D3DX_PI / 3.0f);
	D3DXMatrixTranslation(&matT, size * 2, size, -size);
	mat = matS * matR * matT;
	MakePlane(vecOut, vecWall, mat);

	D3DXMatrixScaling(&matS, size, size, size);
	D3DXMatrixRotationY(&matR, -D3DX_PI / 3.0f);
	D3DXMatrixTranslation(&matT, -size * 2, size, -size);
	mat = matS * matR * matT;
	MakePlane(vecOut, vecWall, mat);
}
