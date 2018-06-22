#include "stdAfx.h"
#include "UIButton.h"
#include "UIText.h"

UIButton::UIButton(IUIButtonDelegate * pDelegate, LPD3DXSPRITE pSprite, int uiTag)
	: IUIObject(pSprite, uiTag)
	, m_pDelegate(pDelegate)
	, m_eButtonState(E_NORMAL)
{
}


UIButton::~UIButton()
{
  
}


void UIButton::Update()
{
	IUIObject::Update();
	
	RECT rect;
	GetFinalRect(&rect);

	POINT mousePoint;
	GetCursorPos(&mousePoint);
	ScreenToClient(DX::GetInfo().hWnd, &mousePoint);

	if (PtInRect(&rect, mousePoint))
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_eButtonState == E_MOUSEOVER)
			{
				m_eButtonState = E_SELECTED;
			}
		}
		else
		{
			if (m_eButtonState == E_SELECTED)
			{
				if (m_pDelegate)
					m_pDelegate->OnClick(this);
			}
			m_eButtonState = E_MOUSEOVER;
		}
	}
	else
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{

		}
		else
		{
			m_eButtonState = E_NORMAL;
		}
	}
}

void UIButton::Render()
{
	if (m_aTexture[m_eButtonState]) 
	{
		RECT rc;
		SetRect(&rc, 0, 0, m_size.x, m_size.y);

		m_pSprite->Draw(m_aTexture[m_eButtonState], &rc, &m_pivot, &m_combinedPos, m_color);
	}

	IUIObject::Render();
}

void UIButton::SetTexture(LPCTSTR normal, LPCTSTR mouseOver, LPCTSTR selected)
{
	D3DXIMAGE_INFO imageInfo;
	m_aTexture[E_NORMAL] = TextureManager::Get()->GetTexture(normal, &imageInfo);
	m_aTexture[E_MOUSEOVER] = TextureManager::Get()->GetTexture(mouseOver, &imageInfo);
	m_aTexture[E_SELECTED] = TextureManager::Get()->GetTexture(selected, &imageInfo);

	m_size.x = imageInfo.Width;
	m_size.y = imageInfo.Height;
}

void UIButton::SetText(LPD3DXFONT font, LPCTSTR text)
{
	pText = new UIText(font, m_pSprite);
	this->AddChild(pText);
	pText->m_text = text;
	pText->m_size = m_size;
}
