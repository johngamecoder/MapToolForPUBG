#include "stdafx.h"
#include "TextureBlending.h"


TextureBlending::TextureBlending()
{
	m_pTex = NULL;
	m_pTex1 = NULL;
}


TextureBlending::~TextureBlending()
{
	
}

void TextureBlending::Init()
{
	D3DCOLOR c = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_vecVertex.push_back(VERTEX_PCT(D3DXVECTOR3(-1, -1, 0), c, D3DXVECTOR2(0, 1)));
	m_vecVertex.push_back(VERTEX_PCT(D3DXVECTOR3(-1,  1, 0), c, D3DXVECTOR2(0, 0)));
	m_vecVertex.push_back(VERTEX_PCT(D3DXVECTOR3( 1,  1, 0), c, D3DXVECTOR2(1, 0)));

	m_vecVertex.push_back(VERTEX_PCT(D3DXVECTOR3(-1, -1, 0), c, D3DXVECTOR2(0, 1)));
	m_vecVertex.push_back(VERTEX_PCT(D3DXVECTOR3( 1,  1, 0), c, D3DXVECTOR2(1, 0)));
	m_vecVertex.push_back(VERTEX_PCT(D3DXVECTOR3( 1, -1, 0), c, D3DXVECTOR2(1, 1)));
	
	m_pTex = TextureManager::Get()->GetTexture(ASSET_PATH + _T("Textures/ham1.png"));
	m_pTex1 = TextureManager::Get()->GetTexture(ASSET_PATH + _T("Textures/particle/particle1.png"));

	
}

void TextureBlending::Update()
{
}

void TextureBlending::Render()
{
	DX::GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	DX::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	DX::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	DX::GetDevice()->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	DX::GetDevice()->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	/*
	DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);

	DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	DX::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DX::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//DX::GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0x40);
	DX::GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	

	//D3DTOP_SUBTRACT
	//D3DTOP_DISABLE
	//D3DTOP_MODULATE
	//D3DTOP_SELECTARG1

	//Texture State 0
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	
	//Texture State 1
	DX::GetDevice()->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	DX::GetDevice()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	DX::GetDevice()->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	DX::GetDevice()->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);


	DX::GetDevice()->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_ADD);
	DX::GetDevice()->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DX::GetDevice()->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	*/
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	DX::GetDevice()->SetFVF(VERTEX_PCT::FVF);
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);
	DX::GetDevice()->SetTexture(0, m_pTex);
	//DX::GetDevice()->SetTexture(1, m_pTex1);
	DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(VERTEX_PCT));

	DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
}
