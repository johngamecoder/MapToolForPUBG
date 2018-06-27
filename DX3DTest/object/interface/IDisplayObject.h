#pragma once

class IDisplayObject : public IObject
{
protected:
	WORD					m_tag;
	D3DXVECTOR3				m_pos;
	D3DXVECTOR3				m_rot;
    D3DXMATRIXA16			m_matWorld;
	IDisplayObject*			m_pParent;
	vector<IDisplayObject*> m_vecChild;
//public:
    //string name;

public:

	IDisplayObject();
	virtual ~IDisplayObject();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void AddChild(IDisplayObject* pChild);
	virtual void Release() override;

	const D3DXVECTOR3&	GetPosition() { return m_pos; }
	void				SetPosition(D3DXVECTOR3* pos) { m_pos = *pos; }
	const D3DXVECTOR3&	GetRotation() { return m_rot; }
	const D3DXMATRIXA16& GetWorldMatrix() { return m_matWorld; }
    void SetWorldMatrix(const D3DXMATRIXA16& matWorld) { m_matWorld = matWorld; }
};

