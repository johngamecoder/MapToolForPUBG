#pragma once

class BoxCollider 
{
private:
    vector<VERTEX_PC>		m_vertices;
    D3DXVECTOR3             m_vMax;


public:
    string              m_Parentname;
    int                 m_index;

    D3DXMATRIXA16       m_mParentTransform;

    D3DXMATRIXA16 m_mTransform;
    D3DXMATRIXA16 m_matWorld;


public:

    BoxCollider(string name);
    ~BoxCollider();

    // Inherited via IDisplayObject
    void Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max) ;
    void Update();
    void Render();

    D3DXVECTOR3& GetCenter() 
    {
        D3DXVECTOR3 vCenter(0, 0, 0);
        D3DXVec3TransformCoord(&vCenter, &vCenter, &m_matWorld);
        return vCenter;
    }
    D3DXVECTOR3& GetExtent()
    {
        D3DXVECTOR3 vCenter = GetCenter();
        D3DXVec3TransformCoord(&m_vMax, &m_vMax, &m_matWorld);
        D3DXVECTOR3 vExtent = m_vMax - vCenter;
        return vExtent;
    }

    void SetVertex(const D3DXVECTOR3& min, const D3DXVECTOR3& max);
    void SetMatrix(const D3DXMATRIXA16& transform, const D3DXMATRIXA16& parentTransform);
};

