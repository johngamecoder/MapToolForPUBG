#pragma once
#include "stdafx.h"

class MipmapFilter : public IDisplayObject
{
	LPDIRECT3DTEXTURE9	m_pTexMipmap;
	vector<VERTEX_PT>	m_vertices;
	UINT				m_vtxStride;
	DWORD				m_vtxFVF;

public:
	MipmapFilter()
	{
		m_pTexMipmap = NULL;
	}

	~MipmapFilter() {}

	virtual void Init() override
	{
		m_vtxStride = sizeof(VERTEX_PT);
		m_vtxFVF = VERTEX_PT::FVF;

		for (int i = 0; i < g_vecQuadIndex.size(); i++)
		{
			m_vertices.push_back(VERTEX_PT(g_vecQuadVertex[g_vecQuadIndex[i]], g_vecQuadUV[g_vecQuadIndex[i]]));
		}
		
		D3DXMATRIXA16 matS, matR;
		D3DXMatrixScaling(&matS, 20, 20, 0);
		D3DXMatrixRotationX(&matR, D3DX_PI / 2);
		for (auto &p : m_vertices)
			D3DXVec3TransformCoord(&p.p, &p.p, &(matS * matR));

		vector<LPDIRECT3DTEXTURE9> tex;
		tex.push_back(TextureManager::Get()->GetTexture(ASSET_PATH + _T("textures/mipmap/png256.png")));
		tex.push_back(TextureManager::Get()->GetTexture(ASSET_PATH + _T("textures/mipmap/png128.png")));
		tex.push_back(TextureManager::Get()->GetTexture(ASSET_PATH + _T("textures/mipmap/png64.png")));
		tex.push_back(TextureManager::Get()->GetTexture(ASSET_PATH + _T("textures/mipmap/png32.png")));
		tex.push_back(TextureManager::Get()->GetTexture(ASSET_PATH + _T("textures/mipmap/png16.png")));
		
		m_pTexMipmap = TextureManager::Get()->GetTexture(ASSET_PATH + _T("textures/mipmap/png512.png"));
		
		LPDIRECT3DSURFACE9 src = NULL, dest = NULL;
		
		for (size_t i = 0; i < tex.size(); i++)
		{
			tex[i]->GetSurfaceLevel(0, &src);
			m_pTexMipmap->GetSurfaceLevel(i + 1, &dest);

			D3DXLoadSurfaceFromSurface(dest, NULL, NULL, src, NULL, NULL, D3DX_DEFAULT, 0);

			src->Release();
			dest->Release();
		}

		DX::GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		DX::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		DX::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	}
	virtual void Update() override {};
	virtual void Render() override
	{
		DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		DX::GetDevice()->SetFVF(m_vtxFVF);
		DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);
		DX::GetDevice()->SetTexture(0, m_pTexMipmap);
		DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vertices.size() / 3, &m_vertices[0], m_vtxStride);
	}
};