#pragma once
#include "stdafx.h"
#include "Ray.h"

class Picking : public IDisplayObject
{
private:
	LPD3DXMESH				m_pMesh;
	vector<BoundingSphere*>	m_pBoundingSpheres;
	
public:
	Picking()
	{
		m_pMesh = NULL;
	}
	~Picking()
	{
		SAFE_RELEASE(m_pMesh);
		for (auto p : m_pBoundingSpheres)
			SAFE_DELETE(p);
	}

	virtual void Init()
	{
		int dim = 6;
		float offset = 4;
		float half = (dim - 1) * offset / 2.0f;
		float radius = 0.6f;
		D3DXCreateSphere(DX::GetDevice(), radius, 10, 10, &m_pMesh, NULL);

		m_pBoundingSpheres.reserve(dim * dim * dim);
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				for (int k = 0; k < dim; k++)
				{
					BoundingSphere* s = new BoundingSphere(D3DXVECTOR3(
						i * offset - half, j * offset - half, k * offset - half), radius);
					m_pBoundingSpheres.push_back(s);
				}
			}
		}
	}

	virtual void Update()
	{
		if (Mouse::Get()->ButtonDown(Mouse::RBUTTON))
		{
			D3DXVECTOR3 pos = Mouse::Get()->GetPosition();
			Ray r = Ray::RayAtWorldSpace(pos.x, pos.y);
			CalcPickedSphere(r);
			//CalcClosestPickedSphere(r);
		}
	}

	virtual void Render()
	{
		DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);

		for (auto p : m_pBoundingSpheres)
		{
			if (p->isPicked)
			{
				DX::GetDevice()->SetMaterial(&DXUtil::RED_MTRL);
			}
			else
			{
				DX::GetDevice()->SetMaterial(&DXUtil::WHITE_MTRL);
			}
			D3DXMATRIXA16 mat;
			D3DXMatrixTranslation(&mat, p->center.x, p->center.y, p->center.z);
			DX::GetDevice()->SetTransform(D3DTS_WORLD, &mat);
			DX::GetDevice()->SetTexture(0, NULL);
			m_pMesh->DrawSubset(0);
		}
	}

	void CalcPickedSphere(Ray &r)
	{
		for (auto p : m_pBoundingSpheres)
		{
			p->isPicked = r.CalcIntersectSphere(p);
		}
	}

	void CalcClosestPickedSphere(Ray &r)
	{
		BoundingSphere* sphere = NULL;
		float minDistance = FLT_MAX;
		float intersectionDistance;
		for (auto p : m_pBoundingSpheres)
		{
			p->isPicked = false;
			if (r.CalcIntersectSphere(p) == true)
			{
				intersectionDistance =
					D3DXVec3Length(&(p->center - r.m_pos));
				if (intersectionDistance < minDistance)
				{
					minDistance = intersectionDistance;
					sphere = p;
				}
			}
		}
		if (sphere != NULL)
		{
			sphere->isPicked = true;
		}
	}
};

