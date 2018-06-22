#pragma once
#include "stdafx.h"

class ColorShader : public IDisplayObject
{
private:
	LPD3DXMESH	 m_pMesh;
	LPD3DXEFFECT m_pEffect;
	
public:
	ColorShader();
	~ColorShader();

	void Init();
	void Update();
	void Render();
};