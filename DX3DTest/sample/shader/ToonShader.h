#pragma once
#include "stdafx.h"

class ToonShader : public IDisplayObject
{
private:
	LPD3DXMESH	 m_pMesh;
	LPD3DXEFFECT m_pEffect;
	LPDIRECT3DTEXTURE9 m_pTex;

public:
	ToonShader();
	~ToonShader();

	void Init();
	void Update();
	void Render();
};