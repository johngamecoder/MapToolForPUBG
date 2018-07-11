#include "stdAfx.h"
#include "Grid.h"
#include "Pyramid.h"

Grid::Grid()
{

}

Grid::~Grid()
{
	for (auto p : m_vecPyramid)
	{
		SAFE_RELEASE(p);
	}
}

void Grid::Init()
{
	float interval = 1.0f;
	int numHalfLine = 15;
	float halfLength = interval * numHalfLine;
	D3DCOLOR c;
	float tmpInterval;
	for (int i = 1; i <= numHalfLine; ++i)
	{
		tmpInterval = interval * i;
		c = (i % 5 == 0) ? D3DCOLOR_XRGB(255, 255, 255) : D3DCOLOR_XRGB(128, 128, 128);
		m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-halfLength, 0.0f,  tmpInterval), c));
		m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3( halfLength, 0.0f,  tmpInterval), c));
		m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-halfLength, 0.0f, -tmpInterval), c));
		m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3( halfLength, 0.0f, -tmpInterval), c));
		m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3( tmpInterval, 0.0f, -halfLength), c));
		m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3( tmpInterval, 0.0f,  halfLength), c));
		m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-tmpInterval, 0.0f, -halfLength), c));
		m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-tmpInterval, 0.0f,  halfLength), c));
	}
	c = D3DCOLOR_XRGB(255, 0, 0);
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(-halfLength, 0.0f, 0.0f), c));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3( halfLength, 0.0f, 0.0f), c));

	c = D3DCOLOR_XRGB(0, 255, 0);
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(0.0f, -halfLength, 0.0f), c));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(0.0f,  halfLength, 0.0f), c));

	c = D3DCOLOR_XRGB(0, 0, 255);
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(0.0f, 0.0f, -halfLength), c));
	m_vecVertex.push_back(VERTEX_PC(D3DXVECTOR3(0.0f, 0.0f,  halfLength), c));

	Pyramid* pPyramid;
	D3DXMATRIXA16 matS, matR, matWorld;
	D3DXMatrixScaling(&matS, 10.0f, 200.0f, 10.0f);

	D3DXMatrixRotationZ(&matR, D3DX_PI / 2.0f);
	matWorld = matS * matR;
	pPyramid = new Pyramid(D3DCOLOR_XRGB(255, 0, 0), matWorld);
	pPyramid->Init();
	m_vecPyramid.push_back(pPyramid);

	D3DXMatrixRotationX(&matR, D3DX_PI);
	matWorld = matS * matR;
	pPyramid = new Pyramid(D3DCOLOR_XRGB(0, 255, 0), matWorld);
	pPyramid->Init();
	m_vecPyramid.push_back(pPyramid);
	
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
	matWorld = matS * matR;
	pPyramid = new Pyramid(D3DCOLOR_XRGB(0, 0, 255), matWorld);
	pPyramid->Init();
	m_vecPyramid.push_back(pPyramid);

    D3DXMATRIXA16 s;
    D3DXMatrixScaling(&s, 100.0f, 0.0f, 100.0f);
    m_matWorld = s * m_matWorld;
}

void Grid::Update()
{
    //D3DXMATRIX matS, matR, matT;
    ////static bool ShowGrid =false;
    ////m_bShowGrid = ShowGrid;
    //ImGui::Begin("Transformation");
    //if (ImGui::Button("Show Grid"))
    //    m_bShowGrid = !m_bShowGrid;

    //////Scaling
    ////static float xS, yS, zS;
    ////ImGui::Text("Scaling");
    ////ImGui::SliderFloat("x", &xS, 1.0f, 10.0f);
    ////ImGui::SliderFloat("y", &yS, 1.0f, 10.0f);
    ////ImGui::SliderFloat("z", &zS, 1.0f, 10.0f);
    ////D3DXMatrixScaling(&matS, xS, yS, zS);
    ////
    //////Rotation
    ////static float xR, yR, zR;
    ////ImGui::Text("Rotation");
    ////ImGui::SliderFloat("x", &xR, 0.0f, 100.0f);
    ////ImGui::SliderFloat("y", &yR, 0.0f, 100.0f);
    ////ImGui::SliderFloat("z", &zR, 0.0f, 100.0f);
    ////D3DXMatrixRotationYawPitchRoll(&matR, yR, xR, zR);

    ////translation
    //static float xT, yT, zT;
    //ImGui::Text("Translation");
    //ImGui::SliderFloat("x", &xT, -10.0f, 10.0f);
    //ImGui::SliderFloat("y", &yT, -10.0f, 10.0f);
    //ImGui::SliderFloat("z", &zT, -10.0f, 10.0f);
    //D3DXMatrixTranslation(&matT, xT, yT, zT);

    //m_matWorld = /*matS * matR **/ matT;
    //ImGui::End();
}

void Grid::Render()
{
    //if (m_bShowGrid)
    //    return;
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);
	DX::GetDevice()->SetTexture(0, NULL);
	DX::GetDevice()->SetFVF(VERTEX_PC::FVF);
	DX::GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 
		m_vecVertex.size() / 2, &m_vecVertex[0], sizeof(VERTEX_PC));

	for (auto p : m_vecPyramid)
	{
		SAFE_RENDER(p);
	}
}
