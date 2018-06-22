#pragma once

class SampleUI;

class SceneUI : public IScene
{
public:
	SampleUI *        	m_pUI;
	VERTEX_PT           m_aVertex[3];
	LPDIRECT3DTEXTURE9  m_pTex;
	LPD3DXSPRITE		m_pSpr;
	D3DXIMAGE_INFO	  	m_stImageInfo;
	LPDIRECT3DTEXTURE9	m_pTex4Spr;
	LPD3DXFONT			m_pFont;

	SceneUI();
	~SceneUI();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void InitTexture();
	void RenderTexture();
	void InitSprite();
	void RenderSprite();
	void InitFont();
	void RenderFont();
};

