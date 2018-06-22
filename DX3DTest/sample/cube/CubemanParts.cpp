#include "stdafx.h"
#include "CubemanParts.h"


CubemanParts::CubemanParts(float rotXSpeed)
{
	m_rotXSpeed = rotXSpeed;
}


CubemanParts::~CubemanParts()
{
	
}

void CubemanParts::Init()
{
	
}

void CubemanParts::Init(D3DXMATRIXA16* pMat, vector<vector<int>> &vecUV)
{
	m_pTex = TextureManager::Get()->GetTexture(ASSET_PATH + _T("Textures/cubeman/naked.png"));

	vector<D3DXVECTOR3> vecVertex;
	for (int i = 0; i < g_vecCubeVertex.size(); i++)
	{
		vecVertex.push_back(g_vecCubeVertex[i]);
	}
	
	if (pMat)
	{
		for (size_t i = 0; i < vecVertex.size(); ++i)
		{
			D3DXVec3TransformCoord(&vecVertex[i], &vecVertex[i], pMat);
		}
	}
	
	//SetPCVertex(m_vecPCVertex, vecVertex);
	//SetPTVertex(m_vecPTVertex, vecVertex, vecUV);
	SetPNTVertex(m_vecPNTVertex, vecVertex, vecUV);
}


void CubemanParts::Update()
{
	D3DXMATRIXA16 matR, matT;

	//+, - 45도로 각도 제한
	if (m_isMoving)
		m_rotXAngle += m_rotXSpeed;

	if (m_rotXAngle < -D3DX_PI / 4.0f)
	{
		m_rotXSpeed *= -1;
		m_rotXAngle = -D3DX_PI / 4.0f;
	}

	if (m_rotXAngle >  D3DX_PI / 4.0f)
	{
		m_rotXSpeed *= -1;
		m_rotXAngle =  D3DX_PI / 4.0f;
	}

	D3DXMatrixRotationX(&matR, m_rotXAngle);
	D3DXMatrixTranslation(&matT, m_pos.x, m_pos.y, m_pos.z);

	m_matWorld = matR * matT;

	if (m_pParent)
	{
		m_matWorld = m_matWorld * m_pParent->GetWorldMatrix();
	}

	for (auto child : m_vecChild)
	{
		child->Update();
	}
}

void CubemanParts::Render()
{
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);

	if (m_vecPCVertex.empty() == false)
	{
		DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		DX::GetDevice()->SetTexture(0, NULL);
		
		DX::GetDevice()->SetFVF(VERTEX_PC::FVF);
		DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecPCVertex.size() / 3,
			&m_vecPCVertex[0], sizeof(VERTEX_PC));
	} 
	else if (m_vecPTVertex.empty() == false)
	{
		DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		DX::GetDevice()->SetTexture(0, m_pTex);

		DX::GetDevice()->SetFVF(VERTEX_PT::FVF);
		DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecPTVertex.size() / 3,
			&m_vecPTVertex[0], sizeof(VERTEX_PT));
	}
	else if (m_vecPNTVertex.empty() == false)
	{
		DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
		DX::GetDevice()->SetTexture(0, m_pTex);
		DX::GetDevice()->SetMaterial(&DXUtil::WHITE_MTRL);

		DX::GetDevice()->SetFVF(VERTEX_PNT::FVF);
		DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecPNTVertex.size() / 3,
			&m_vecPNTVertex[0], sizeof(VERTEX_PNT));
	}
	
	for (auto child : m_vecChild)
	{
		child->Render();
	}
}

void CubemanParts::SetPCVertex(vector<VERTEX_PC> &vecOut, vector<D3DXVECTOR3> &vecVertex)
{
	D3DCOLOR red = D3DCOLOR_XRGB(255, 0, 0);
	D3DCOLOR green = D3DCOLOR_XRGB(0, 255, 0);
	D3DCOLOR blue = D3DCOLOR_XRGB(0, 0, 255);
	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);
	D3DCOLOR yellow = D3DCOLOR_XRGB(255, 255, 0);

	vector<VERTEX_PC> vec;
	vec.push_back(VERTEX_PC(vecVertex[0], white));
	vec.push_back(VERTEX_PC(vecVertex[1], yellow));
	vec.push_back(VERTEX_PC(vecVertex[2], green));
	vec.push_back(VERTEX_PC(vecVertex[3], blue));
	vec.push_back(VERTEX_PC(vecVertex[4], white));
	vec.push_back(VERTEX_PC(vecVertex[5], yellow));
	vec.push_back(VERTEX_PC(vecVertex[6], red));
	vec.push_back(VERTEX_PC(vecVertex[7], green));

	for (int i = 0; i < g_vecCubeIndex.size(); i++)
	{
		vecOut.push_back(vec[g_vecCubeIndex[i]]);
	}
}

void CubemanParts::SetPTVertex(vector<VERTEX_PT>& vecOut, vector<D3DXVECTOR3>& vecVertex, vector<vector<int>>& vecUV)
{
	vector<D3DXVECTOR2> vecUV4Vertex;
	MakeUVList(vecUV4Vertex, vecUV);
	
	for (int i = 0; i < g_vecCubeIndex.size(); i++)
	{
		vecOut.push_back(VERTEX_PT(vecVertex[g_vecCubeIndex[i]], vecUV4Vertex[i]));
	}
}


void CubemanParts::SetPNTVertex(vector<VERTEX_PNT> &vecOut, vector<D3DXVECTOR3> &vecVertex, vector<vector<int>> &vecUV)
{
	vector<D3DXVECTOR2> vecUV4Vertex;
	MakeUVList(vecUV4Vertex, vecUV);
	D3DXVECTOR3 n;

	for (int i = 0; i < g_vecCubeIndex.size(); i++)
	{
		if ( i % 6 == 0 )
			DXUtil::ComputeNormal(&n, &vecVertex[g_vecCubeIndex[i]],
				&vecVertex[g_vecCubeIndex[i + 1]], &vecVertex[g_vecCubeIndex[i + 2]]);

		vecOut.push_back(VERTEX_PNT(vecVertex[g_vecCubeIndex[i]], n, vecUV4Vertex[i]));
	}
}


void CubemanParts::MakeUVList(vector<D3DXVECTOR2> &out, vector<vector<int>> &uv)
{
	for (size_t i = 0; i < uv.size(); i++)
	{
		out.push_back(D3DXVECTOR2(uv[i][0] / 64.0f, uv[i][1] / 32.0f));
		out.push_back(D3DXVECTOR2(uv[i][2] / 64.0f, uv[i][3] / 32.0f));
		out.push_back(D3DXVECTOR2(uv[i][4] / 64.0f, uv[i][5] / 32.0f));

		out.push_back(D3DXVECTOR2(uv[i][0] / 64.0f, uv[i][1] / 32.0f));
		out.push_back(D3DXVECTOR2(uv[i][4] / 64.0f, uv[i][5] / 32.0f));
		out.push_back(D3DXVECTOR2(uv[i][6] / 64.0f, uv[i][7] / 32.0f));
	}
}


void CubemanParts::SetMovingState(bool isMoving)
{
	if (m_isMoving == isMoving)
		return;

	m_isMoving = isMoving;

	if (m_isMoving == false)
	{
		m_rotXAngle = 0;
	}

	for (auto child : m_vecChild)
	{
		static_cast<CubemanParts*>(child)->SetMovingState(m_isMoving);
	}
}
