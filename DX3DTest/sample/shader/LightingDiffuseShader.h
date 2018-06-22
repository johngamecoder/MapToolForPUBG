#pragma once
#include "stdafx.h"

class LightingDiffuseShader : public IDisplayObject
{
protected:
	LPD3DXMESH	 m_pMesh;
	LPD3DXEFFECT m_pEffect;

public:
	LightingDiffuseShader();
	~LightingDiffuseShader();

	void Init();
	void Update();
	void Render();
};

