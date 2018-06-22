#pragma once
#include "stdafx.h"

class ShaderManager
{
public:
	static ShaderManager*	Get();
	static void		Delete();
private:
	static ShaderManager*	instance;

	map<CString, LPD3DXEFFECT> m_pEffects;

	ShaderManager();
	~ShaderManager();
public:
	LPD3DXEFFECT GetShader(LPCTSTR filePath);

private:
	LPD3DXEFFECT LoadShader(LPCTSTR fullPath);
};