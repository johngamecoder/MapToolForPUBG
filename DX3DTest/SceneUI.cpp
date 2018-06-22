#include "stdafx.h"
#include "SceneUI.h"
#include "SampleUI.h"

SceneUI::SceneUI()
	:m_pUI(NULL)
	,m_pSpr(NULL)
	,m_pTex4Spr(NULL)
	,m_pTex(NULL)
	,m_pFont(NULL)
{
}


SceneUI::~SceneUI()
{
	SAFE_RELEASE(m_pSpr);
	SAFE_RELEASE(m_pTex4Spr);
	SAFE_RELEASE(m_pTex);
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_pUI);
}

void SceneUI::Init()
{
	//m_pUI = new SampleUI; m_pUI->Init();
	
//#define TestTexture
//#define TestSprite
//#define TestFont

#ifdef TestTexture
  InitTexture();
#endif
#ifdef TestSprite
  InitSprite();
#endif
#ifdef TestFont
  InitFont();
#endif
}

void SceneUI::Update()
{
	SAFE_UPDATE(m_pUI);
}

void SceneUI::Render()
{
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	SAFE_RENDER(m_pUI);
	
#ifdef TestTexture
  RenderTexture();
#endif
#ifdef TestSprite
  RenderSprite();
#endif
#ifdef TestFont
  RenderFont();
#endif
}


void SceneUI::InitTexture()
{
	D3DXCreateTextureFromFile(DX::GetDevice(), _T("resources/images/code.png"), &m_pTex);

	m_aVertex[0].p = D3DXVECTOR3(0, 0, 0);
	m_aVertex[0].t = D3DXVECTOR2(0, 1);
	m_aVertex[1].p = D3DXVECTOR3(0, 1, 0);
	m_aVertex[1].t = D3DXVECTOR2(0, 0);
	m_aVertex[2].p = D3DXVECTOR3(1, 0, 0);
	m_aVertex[2].t = D3DXVECTOR2(1, 1);
	/*
	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixScaling(&mat, 3, 3, 3);

	D3DXVec3TransformCoord(&m_aVertex[0].p, &m_aVertex[0].p, &mat);
	D3DXVec3TransformCoord(&m_aVertex[1].p, &m_aVertex[1].p, &mat);
	D3DXVec3TransformCoord(&m_aVertex[2].p, &m_aVertex[2].p, &mat);
	*/
}


void SceneUI::RenderTexture()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matWorld, 0, 0, 0);
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
	DX::GetDevice()->SetTexture(0, m_pTex);
	DX::GetDevice()->SetFVF(VERTEX_PT::FVF);
	DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, m_aVertex, sizeof(VERTEX_PT));
}


void SceneUI::InitSprite()
{
	/**/
	D3DXCreateTextureFromFileEx(
		DX::GetDevice(),				//LPDIRECT3DDEVICE9 pDevice,
		_T("resources/images/ham1.png"),	//LPCTSTR pSrcFile,
		D3DX_DEFAULT_NONPOW2,	//UINT Width,
		D3DX_DEFAULT_NONPOW2,	//UINT Height,
		D3DX_DEFAULT,		//UINT MipLevels,
		0,					//DWORD Usage,
		D3DFMT_UNKNOWN,		//D3DFORMAT Format,
		D3DPOOL_MANAGED,	//D3DPOOL Pool
		D3DX_FILTER_NONE,	//DWORD Filter
		D3DX_DEFAULT,		//DWORD MipFilter
		D3DCOLOR_XRGB(255, 255, 255),	//D3DCOLOR ColorKey
		&m_stImageInfo,	//D3DXIMAGE_INFO *pSrcInfo
		NULL,			//PALETTEENTRY *pPalette
		&m_pTex4Spr);	//LPDIRECT3DTEXTURE9 *ppTexture
		
	//D3DXCreateTextureFromFile(DX::GetDevice(), _T("resources/images/ham1.png"), &m_pTex4Spr);
	D3DXCreateSprite(DX::GetDevice(), &m_pSpr);
}


void SceneUI::RenderSprite()
{
	RECT clientRect;
	GetClientRect(DX::GetInfo().hWnd, &clientRect);
	
	D3DXMATRIXA16 matR, matT, matWorld;
	static float fAngle = 0.0f;
	//fAngle += 0.1f;
	D3DXMatrixRotationZ(&matR, fAngle);
	D3DXMatrixIdentity(&matT);
	D3DXMatrixTranslation(&matT, clientRect.right - m_stImageInfo.Width, 0, 0);
	matWorld = matR * matT;
	RECT rc;
	SetRect(&rc, 0, 0, m_stImageInfo.Width, m_stImageInfo.Height);
	
	m_pSpr->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_pSpr->SetTransform(&matWorld);
	m_pSpr->Draw(
		m_pTex4Spr,
		&rc,
		//&D3DXVECTOR3(m_stImageInfo.Width / 2, m_stImageInfo.Height / 2, 0),
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		WHITE);

	m_pSpr->End();
}


void SceneUI::InitFont()
{
	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));

	fd.Width = 24;
	fd.Height = 48;
	fd.Weight = FW_NORMAL;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;

	//strcpy_s(fd.FaceName, "±¼¸²Ã¼");
	//strcpy_s(fd.FaceName, "±Ã¼­Ã¼");
	//AddFontResource(_T("resources/font/umberto.ttf"));
	lstrcpy(fd.FaceName, _T("umberto"));
	
	D3DXCreateFontIndirect(DX::GetDevice(), &fd, &m_pFont);
}


void SceneUI::RenderFont()
{
	D3DXFONT_DESC fd;
	m_pFont->GetDesc(&fd);
 
	LPCTCH str = _T("ABCDabcd1234!@#$.");
	RECT clientRect;
	GetClientRect(DX::GetInfo().hWnd, &clientRect);
	UINT offsetX = clientRect.right / 2.0f;
	UINT offsetY = 10;
	RECT rc;
	SetRect(&rc, offsetX, offsetY,
		offsetX + fd.Width * lstrlen(str), offsetY + fd.Height);
	
	m_pFont->DrawText(
		NULL,
		str,
		lstrlen(str),
		&rc,
		DT_LEFT | DT_TOP,// | DT_NOCLIP,
		D3DCOLOR_XRGB(0, 255, 255));
}

