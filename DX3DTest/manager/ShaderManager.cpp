#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager* ShaderManager::instance = NULL;

ShaderManager * ShaderManager::Get()
{
	if (instance == NULL)
		instance = new ShaderManager();

	return instance;
}

void ShaderManager::Delete()
{
	SAFE_DELETE(instance);
}

ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{
	for (auto p : m_pEffects)
		SAFE_RELEASE(p.second);
}

LPD3DXEFFECT ShaderManager::GetShader(LPCTSTR filePath)
{
	if (m_pEffects.find(filePath) == m_pEffects.end())
	{
		m_pEffects[filePath] = LoadShader(filePath);
	}
	return m_pEffects[filePath];
}

LPD3DXEFFECT ShaderManager::LoadShader(LPCTSTR fullPath)
{
	LPD3DXEFFECT pEffect = NULL;
	LPD3DXBUFFER pError = NULL;
	DWORD flags = D3DXSHADER_DEBUG;
	
	D3DXCreateEffectFromFile(DX::GetDevice(), fullPath,
		NULL, NULL, flags, NULL, &pEffect, &pError);
	
	if (!pEffect && pError)
	{
		int size = pError->GetBufferSize();
		LPVOID errorMsg = pError->GetBufferPointer();
		
		if (errorMsg)
		{
			char* str = new char[size];
			sprintf_s(str, size, (const char*)errorMsg, size);
			
			//MessageBoxA(NULL, str, 0, 0);
			OutputDebugStringA((str));
			delete[] str;
		}
	}

	return pEffect;
}
