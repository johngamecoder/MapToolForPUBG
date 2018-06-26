#pragma once


class BoxCollider 
{
private:
    vector<VERTEX_PC>		m_vertices;
    vector<WORD>			m_vecIndex;

    //LPDIRECT3DVERTEXBUFFER9 m_pVB;
    //LPDIRECT3DINDEXBUFFER9  m_pIB;
    //D3DVERTEXBUFFER_DESC	m_VBDesc;	//buffer description
    //D3DINDEXBUFFER_DESC		m_IBDesc;

public:
    string              m_Parentname;
    int                 m_index;
    D3DXVECTOR3         m_vCenter;
    D3DXVECTOR3         m_vExtent;
    D3DXMATRIXA16       m_mParentTransform;
    D3DCOLOR            m_color;
    // row 0 : x, y and z of axis 0
    // row 1 : x, y and z of axis 1
    // row 2 : x, y and z of axis 2
    // row 3 : x, y and z of translation
    D3DXMATRIXA16 m_mTransform;
    D3DXMATRIXA16 m_matWorld;

    //bool m_isRender;
    //D3DXVECTOR3 m_center;   //pos 
    //D3DXVECTOR3 m_vExtent;
    //vector<VERTEX_PC> m_vertices;
    //D3DCOLOR m_color = D3DCOLOR_ARGB(1, 0, 1, 0);
public:

    BoxCollider(string name);
    ~BoxCollider();

    // Inherited via IDisplayObject
    void Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max) ;
    void Update();
    void Render() ;

    void SetVertex(const D3DXVECTOR3& min, const D3DXVECTOR3& max);
    void SetMatrix(const D3DXMATRIXA16& transform, const D3DXMATRIXA16& parentTransform);
    //void SetBuffer(LPDIRECT3DVERTEXBUFFER9 & pVb, LPDIRECT3DINDEXBUFFER9 & pIb, vector<VERTEX_PC>& vecVertex, vector<WORD>& vecIndex);
    
};

