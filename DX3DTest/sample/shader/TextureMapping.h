#pragma once
#include "stdafx.h"

class TextureMapping : public IDisplayObject
{
private:
	LPD3DXMESH	 m_pMesh;
	LPD3DXEFFECT m_pEffect;
	LPDIRECT3DTEXTURE9 m_pTex;
	
public:
	TextureMapping();
	~TextureMapping();

	void Init();
	void Update();
	void Render();
};