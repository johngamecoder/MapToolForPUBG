#pragma once

class BoxCollider 
{
private:
    vector<VERTEX_PC>		m_vertices;
    string*                 m_ParentName;
    D3DXVECTOR3             m_vMax;

public:
    D3DXVECTOR3         m_Position;
    D3DXVECTOR3         m_Rotation;
    D3DXVECTOR3         m_Scale;
    int                 m_index;
    D3DXMATRIXA16       m_mParentTransform;

    D3DXMATRIXA16 m_mTransform;
    D3DXMATRIXA16 m_matWorld;


public:

    BoxCollider(string* name);
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
        D3DXMATRIXA16 matS;
        D3DXMatrixScaling(&matS, m_Scale.x, m_Scale.y, m_Scale.z);
        
        D3DXVECTOR3 vExtent = m_vMax;
        D3DXVec3TransformCoord(&vExtent, &vExtent, &matS);
        return vExtent;
    }

    string* GetParentName()
    {
        return m_ParentName;
    }
    void SetVertex(const D3DXVECTOR3& min, const D3DXVECTOR3& max);
    void SetMatrix(const D3DXMATRIXA16& transform, const D3DXMATRIXA16& parentTransform);
};








//D3DXMATRIXA16 matR, matT;
//D3DXMatrixRotationYawPitchRoll(&matR, m_Rotation.x, m_Rotation.y, m_Rotation.z);
//D3DXMatrixTranslation(&matT, m_Position.x, m_Position.y, m_Position.z);
//D3DXMATRIXA16 matInvR, matInvT;
//D3DXMatrixInverse(&matInvT, NULL, &matT);
//D3DXMatrixInverse(&matInvR, NULL, &matR);
//
////D3DXMATRIXA16 result;
////result = m_matWorld * matInvT * matInvR;
//
//D3DXVECTOR3 max = m_vMax;
//D3DXVec3TransformCoord(&max, &max, &m_matWorld);
//D3DXVec3TransformCoord(&max, &max, &matInvT);
//D3DXVec3TransformCoord(&max, &max, &matInvR);
//D3DXVECTOR3 center = GetCenter();
//D3DXVECTOR3 extent = max - center;
//
//return extent;



//ÈÆÈ¸Çü
//D3DXMATRIX invT, invR;
//D3DXMatrixTranslation(&invT, m_Position.x, m_Position.y, m_Position.z);
//D3DXMatrixRotationYawPitchRoll(&invR, m_Rotation.x, m_Rotation.y, m_Rotation.z);
//D3DXMatrixInverse(&invT, nullptr, &invT);
//D3DXMatrixInverse(&invR, nullptr, &invR);
//
//D3DXMATRIX s = m_matWorld * invR * invT;
//
//
//
//D3DXVECTOR3 extent;
//D3DXMatrixScaling(&s, m_Scale.x, m_Scale.y, m_Scale.z);
//D3DXVec3TransformCoord(&extent, &D3DXVECTOR3(0.5f, 0.5f, 0.5f), &s);
//
//return extent;