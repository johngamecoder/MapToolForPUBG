#pragma once


class BoxCollider : public IDisplayObject
{
private:
    vector<VERTEX_PC>		m_vecVertex;
    vector<WORD>			m_vecIndex;

    LPDIRECT3DVERTEXBUFFER9 m_pVB;
    LPDIRECT3DINDEXBUFFER9  m_pIB;
    D3DVERTEXBUFFER_DESC	m_VBDesc;	//buffer description
    D3DINDEXBUFFER_DESC		m_IBDesc;


    D3DXVECTOR3 m_min;
    D3DXVECTOR3 m_max;

    //bool m_isRender;
    //D3DXVECTOR3 m_center;   //pos 
    //D3DXVECTOR3 m_vExtent;
    //vector<VERTEX_PC> m_vertices;
    //D3DCOLOR m_color = D3DCOLOR_ARGB(1, 0, 1, 0);
public:

    BoxCollider();
    ~BoxCollider();

    // Inherited via IDisplayObject
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render() override;

    void SetVertex(vector<VERTEX_PC>& vecVertexOut, vector<WORD>& vecIndexOut, vector<D3DXVECTOR3> vecPos);

    void SetBuffer(LPDIRECT3DVERTEXBUFFER9 & pVb, LPDIRECT3DINDEXBUFFER9 & pIb, vector<VERTEX_PC>& vecVertex, vector<WORD>& vecIndex);
    
};

