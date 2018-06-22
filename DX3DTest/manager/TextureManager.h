#pragma once

class TextureManager
{
public:
	static TextureManager*	Get();
	static void		Delete();
private:
	static TextureManager*	instance;

	map<CString, LPDIRECT3DTEXTURE9>	m_mapTexture;
	map<CString, D3DXIMAGE_INFO>		m_mapImageInfo;

	TextureManager();
	~TextureManager();

private:
	LPDIRECT3DTEXTURE9 _GetTexture(LPCTSTR filePath, OUT D3DXIMAGE_INFO* pImageInfo = NULL);
public:
	LPDIRECT3DTEXTURE9 GetTexture(LPCTSTR filePath, OUT D3DXIMAGE_INFO* pImageInfo = NULL);
};

