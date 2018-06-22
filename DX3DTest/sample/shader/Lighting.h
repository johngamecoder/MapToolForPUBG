#pragma once
#include "stdafx.h"

class Lighting : public IDisplayObject
{
protected:
	LPD3DXMESH	 m_pMesh;
	LPD3DXEFFECT m_pEffect;

public:
	Lighting();
	~Lighting();

	void Init();
	void Update();
	void Render();
};

