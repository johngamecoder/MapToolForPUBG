#pragma once

#include "IUIObject.h"

class UIButton;
class UIText;
class IUIButtonDelegate
{
public:
	virtual void OnClick(UIButton* pSender) = 0;
};


class UIButton : public IUIObject
{
private:
	enum ButtonState
	{
		E_NORMAL,
		E_MOUSEOVER,
		E_SELECTED,
		E_COUNT
	};
	ButtonState		m_eButtonState;
	
public:
	IUIButtonDelegate * m_pDelegate;
	LPDIRECT3DTEXTURE9	m_aTexture[E_COUNT];
	UIText* pText;
	UIButton(IUIButtonDelegate * pDelegate, LPD3DXSPRITE pSprite, int uiTag = -1);
	virtual ~UIButton();

	virtual void Update() override;
	virtual void Render() override;

	void SetTexture(LPCTSTR normal, LPCTSTR mouseOver, LPCTSTR selected);
	void SetText(LPD3DXFONT font, LPCTSTR text);
};

