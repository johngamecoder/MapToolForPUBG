#pragma once

class IUIObject : public IDisplayObject
{
protected:
	LPD3DXSPRITE m_pSprite;

public:
	int			m_uiTag;
	D3DXVECTOR3	m_combinedPos;
	D3DXVECTOR3 m_pivot;
	D3DXVECTOR2	m_size;
	D3DCOLOR	m_color;
	
	bool		m_bDrawBorder;

	IUIObject(LPD3DXSPRITE pSprite, int uiTag = -1);
	virtual ~IUIObject();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void UpdateCombinedPosition();
	void DrawBorder();
	IUIObject* FindChildByUITag(int nTag);
	void GetFinalRect(RECT *rect);
	const D3DXVECTOR3& GetCombinedPosition() { return m_combinedPos; }
};

