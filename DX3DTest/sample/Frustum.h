#pragma once
#include "stdafx.h"

class Frustum : public IDisplayObject
{
	vector<D3DXVECTOR3>	m_vecProjVtx;
	vector<D3DXVECTOR3>	m_vecWorldVtx;
	vector<D3DXPLANE>	m_vecPlane;
	LPD3DXMESH			m_pMesh;
	vector<BoundingSphere*> m_pBoundingShperes;

public:
	Frustum()
	{
		m_pMesh = NULL;
	}
	~Frustum()
	{
		SAFE_RELEASE(m_pMesh);
	}

	void Init()
	{
		m_vecProjVtx.push_back(D3DXVECTOR3(-1, 1, 1));	//ÁÂ»óÈÄ
		m_vecProjVtx.push_back(D3DXVECTOR3(1, 1, 1));	//¿ì»óÈÄ
		m_vecProjVtx.push_back(D3DXVECTOR3(-1, 1, 0));	//ÁÂ»óÀü
		m_vecProjVtx.push_back(D3DXVECTOR3(1, 1, 0));	//¿ì»óÀü
		m_vecProjVtx.push_back(D3DXVECTOR3(-1, -1, 1));	//ÁÂÇÏÈÄ
		m_vecProjVtx.push_back(D3DXVECTOR3(1, -1, 1));	//¿ìÇÏÈÄ
		m_vecProjVtx.push_back(D3DXVECTOR3(-1, -1, 0));	//ÁÂÇÏÀü
		m_vecProjVtx.push_back(D3DXVECTOR3(1, -1, 0));	//¿ìÇÏÀü

		m_vecWorldVtx.resize(8);
		m_vecPlane.resize(6);

		int dim = 6;
		float offset = 4;
		float half = (dim - 1) * offset / 2.0f;
		float radius = 0.6f;
		D3DXCreateSphere(DX::GetDevice(), radius, 10, 10, &m_pMesh, NULL);

		m_pBoundingShperes.reserve(dim * dim * dim);
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				for (int k = 0; k < dim; k++)
				{
					BoundingSphere* s = new BoundingSphere(D3DXVECTOR3(
						i * offset - half, j * offset - half, k * offset - half), radius);
					m_pBoundingShperes.push_back(s);
				}
			}
		}

		UpdateFrustum();
	}
	
	void Update()
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x0001)
		{
			UpdateFrustum();
		}
	}
	
	void Render()
	{
		DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
		DX::GetDevice()->SetMaterial(&DXUtil::GREEN_MTRL);

		for (auto p : m_pBoundingShperes)
		{
			if (IsSphereInsideFrustum(p) == true)
			{
				D3DXMATRIXA16 mat;
				D3DXMatrixTranslation(&mat, p->center.x, p->center.y, p->center.z);
				DX::GetDevice()->SetTransform(D3DTS_WORLD, &mat);
				DX::GetDevice()->SetTexture(0, NULL);
				m_pMesh->DrawSubset(0);
			}
		}
	}

	void UpdateFrustum()
	{
		D3DXMATRIXA16 matProj, matView;
		DX::GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);
		DX::GetDevice()->GetTransform(D3DTS_VIEW, &matView);

		for (size_t i = 0; i < m_vecWorldVtx.size(); ++i)
		{
			D3DXVec3Unproject(&m_vecWorldVtx[i], &m_vecProjVtx[i],
				NULL, &matProj, &matView, NULL);
		}
		//±ÙÆò¸é//ÁÂ»óÀü//¿ì»óÀü//ÁÂÇÏÀü
		D3DXPlaneFromPoints(&m_vecPlane[0],
			&m_vecWorldVtx[2], &m_vecWorldVtx[3], &m_vecWorldVtx[6]);
		//¿øÆò¸é//¿ì»óÈÄ//ÁÂ»óÈÄ//¿ìÇÏÈÄ
		D3DXPlaneFromPoints(&m_vecPlane[1],
			&m_vecWorldVtx[1], &m_vecWorldVtx[0], &m_vecWorldVtx[5]);
		//ÁÂÆò¸é//ÁÂ»óÈÄ//ÁÂ»óÀü//ÁÂÇÏÈÄ
		D3DXPlaneFromPoints(&m_vecPlane[2],
			&m_vecWorldVtx[0], &m_vecWorldVtx[2], &m_vecWorldVtx[4]);
		//¿ìÆò¸é//¿ì»óÀü//¿ì»óÈÄ//¿ìÇÏÀü
		D3DXPlaneFromPoints(&m_vecPlane[3],
			&m_vecWorldVtx[3], &m_vecWorldVtx[1], &m_vecWorldVtx[7]);
		//»óÆò¸é//ÁÂ»óÈÄ//¿ì»óÈÄ//ÁÂ»óÀü
		D3DXPlaneFromPoints(&m_vecPlane[4],
			&m_vecWorldVtx[0], &m_vecWorldVtx[1], &m_vecWorldVtx[2]);
		//ÇÏÆò¸é//ÁÂÇÏÀü//¿ìÇÏÀü//ÁÂÇÏÈÄ
		D3DXPlaneFromPoints(&m_vecPlane[5],
			&m_vecWorldVtx[6], &m_vecWorldVtx[7], &m_vecWorldVtx[4]);
	}

	bool IsSphereInsideFrustum(BoundingSphere* pSphere)
	{
		for (auto p : m_vecPlane)
		{
			if (D3DXPlaneDotCoord(&p, &pSphere->center) > pSphere->radius)
			{
				return false;
			}
		}

		return true;
	}
};

