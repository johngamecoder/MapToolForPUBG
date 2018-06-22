#include "stdafx.h"
#include "TextureManager.h"

TextureManager* TextureManager::instance = NULL;

TextureManager * TextureManager::Get()
{
	if (instance == NULL)
		instance = new TextureManager();

	return instance;
}

void TextureManager::Delete()
{
	SAFE_DELETE(instance);
}


TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for (auto it : m_mapTexture)
	{
		SAFE_RELEASE(it.second);
	}
	m_mapTexture.clear();
	m_mapImageInfo.clear();
}


LPDIRECT3DTEXTURE9 TextureManager::_GetTexture(LPCTSTR filePath, OUT D3DXIMAGE_INFO * pImageInfo)
{
	if (pImageInfo)
	{
		if (m_mapTexture.find(filePath) == m_mapTexture.end() ||
			m_mapImageInfo.find(filePath) == m_mapImageInfo.end())
		{
			SAFE_RELEASE(m_mapTexture[filePath]);

			D3DXCreateTextureFromFileEx(
				DX::GetDevice(),
				filePath,
				0,	//Width : 0 또는 D3DX_DEFAULT - 파일 사이즈 사용
				0,	//Height : D3DX_DEFAULT_NONPOW2 - 원본 사이즈 사용(2^n 으로 변경 안됨)
				1,	//MipLevels : D3DX_DEFAULT 이면 완전한 밉맵 체인 생성
				0,	//Usage	: D3DUSAGE_RENDERTARGET 렌더링 타겟으로서 사용
				D3DFMT_UNKNOWN,	//Format : D3DFMT_UNKNOWN 파일포맷 사용
				D3DPOOL_MANAGED,	//Pool
				D3DX_FILTER_NONE,	//Filter
				D3DX_DEFAULT,	//MipFilter
				0,	//D3DCOLOR_ARGB(10, 64, 64, 64),	//ColorKey : 키값을 투명으로 만듬
				&m_mapImageInfo[filePath],	//pSrcInfo
				NULL,	//pPalette
				&m_mapTexture[filePath]);	//ppTexture
		}
		*pImageInfo = m_mapImageInfo[filePath];
	}
	else
	{
		if (m_mapTexture.find(filePath) == m_mapTexture.end())
		{
			if (FAILED(D3DXCreateTextureFromFile(DX::GetDevice(), filePath, &m_mapTexture[filePath])));
				//assert(false && "Loading texture is failed");
		}
	}

	return m_mapTexture[filePath];
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(LPCTSTR filePath, OUT D3DXIMAGE_INFO* pImageInfo)
{
	if (lstrlen(filePath) == 0) return NULL;

	return _GetTexture(filePath, pImageInfo);
}
