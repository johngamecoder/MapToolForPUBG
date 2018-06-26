#include "stdafx.h"
#include "BoxCollider.h"
#define CUBE_INDEX_SIZE 36
#define CUBE_VERTEX_SIZE 8



BoxCollider::BoxCollider(string name, const D3DXMATRIXA16& parentTransform)
    : m_Parentname(name)
    , m_vExtent(0.0f, 0.0f, 0.0f)
    , m_color(D3DCOLOR_XRGB(0, 255, 0))
    , m_mParentTransform(parentTransform)
{
    m_mTransform = m_mParentTransform;
}

BoxCollider::~BoxCollider()
{
    //SAFE_RELEASE(m_pVB);
    //SAFE_RELEASE(m_pIB);
}

void BoxCollider::Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max)
{
    m_vCenter = (min + max) * 0.5f;
    m_vExtent = max - m_vCenter;
}

void BoxCollider::Update(const D3DXMATRIXA16& transform/*, const D3DXMATRIXA16& parentTransform*/)
{
    D3DXMATRIX InverseMatrixOfCurrent, TM;
    D3DXMatrixInverse(&InverseMatrixOfCurrent, nullptr, &m_mTransform);
    TM = InverseMatrixOfCurrent * transform;
    D3DXVec3TransformCoord(&m_vCenter, &m_vCenter, &TM);

    //m_mTransform =transform;
    m_mTransform = transform * m_mParentTransform;
}

void BoxCollider::Render()
{
    SetVertex(-m_vExtent, m_vExtent);

    vector<WORD> indices =
    {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7,
    };

    const auto pD = DX::GetDevice();
    pD->SetRenderState(D3DRS_LIGHTING, false);
    pD->SetTransform(D3DTS_WORLD, &m_mTransform);
    pD->SetTexture(0, nullptr);
    pD->SetFVF(VERTEX_PC::FVF);
    pD->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, m_vertices.size(),
        indices.size() / 2, indices.data(), D3DFMT_INDEX16, m_vertices.data(),
        sizeof VERTEX_PC);
    pD->SetRenderState(D3DRS_LIGHTING, true);
    //g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
    //	m_vecVertex.size() / 3,
    //	&m_vecVertex[0], sizeof(VERTEX_PC));
    //g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_vecVertex.size(), m_vecIndex.size() / 3,&m_vecIndex[0],D3DFMT_INDEX16,&m_vecVertex[0],sizeof(VERTEX_PC));
   /* pD->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PC));
    pD->SetIndices(m_pIB);
    pD->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_VBDesc.Size, 0, m_IBDesc.Size / 2);*/
}

void BoxCollider::SetVertex(const D3DXVECTOR3 & min, const D3DXVECTOR3 & max)
{
    m_vertices.resize(8);
    m_vertices[0] = VERTEX_PC(D3DXVECTOR3(min.x, min.y, min.z), m_color);
    m_vertices[1] = VERTEX_PC(D3DXVECTOR3(min.x, max.y, min.z), m_color);
    m_vertices[2] = VERTEX_PC(D3DXVECTOR3(max.x, max.y, min.z), m_color);
    m_vertices[3] = VERTEX_PC(D3DXVECTOR3(max.x, min.y, min.z), m_color);
    m_vertices[4] = VERTEX_PC(D3DXVECTOR3(min.x, min.y, max.z), m_color);
    m_vertices[5] = VERTEX_PC(D3DXVECTOR3(min.x, max.y, max.z), m_color);
    m_vertices[6] = VERTEX_PC(D3DXVECTOR3(max.x, max.y, max.z), m_color);
    m_vertices[7] = VERTEX_PC(D3DXVECTOR3(max.x, min.y, max.z), m_color);
}

//void BoxCollider::SetVertex(vector<VERTEX_PC>& vecVertexOut, vector<WORD>& vecIndexOut, vector<D3DXVECTOR3> vecPos)
//{
//    WORD g_aCubeIndex[24] = {
//        //0, 1, 2, 0, 2, 3,	// 후
//        //7, 6, 5, 7, 5, 4,	// 전
//        //4, 5, 1, 4, 1, 0,	// 좌
//        //3, 2, 6, 3, 6, 7,	// 우
//        //1, 5, 6, 1, 6, 2,	// 상
//        //4, 0, 3, 4, 3, 7	// 하
//        0,1,1,2,2,3,3,0,
//        0,4,1,5,2,6,3,7,
//        4,7,7,6,6,5,5,4,
//    };
//    D3DCOLOR green = D3DCOLOR_XRGB(0, 255, 0);
//
//    vecVertexOut.push_back(VERTEX_PC(vecPos[0], green));
//    vecVertexOut.push_back(VERTEX_PC(vecPos[1], green));
//    vecVertexOut.push_back(VERTEX_PC(vecPos[2], green));
//    vecVertexOut.push_back(VERTEX_PC(vecPos[3], green));
//    vecVertexOut.push_back(VERTEX_PC(vecPos[4], green));
//    vecVertexOut.push_back(VERTEX_PC(vecPos[5], green));
//    vecVertexOut.push_back(VERTEX_PC(vecPos[6], green));
//    vecVertexOut.push_back(VERTEX_PC(vecPos[7], green));
//
//    for (size_t i = 0; i < CUBE_INDEX_SIZE; i++)
//    {
//        vecIndexOut.push_back(g_aCubeIndex[i]);
//    }
//}
//
//void BoxCollider::SetBuffer(LPDIRECT3DVERTEXBUFFER9 & pVb, LPDIRECT3DINDEXBUFFER9 & pIb, vector<VERTEX_PC>& vecVertex, vector<WORD>& vecIndex)
//{																						 //다랙이 pool관리 해주세요
//    DX::GetDevice()->CreateVertexBuffer(vecVertex.size() * sizeof(VERTEX_PC), 0, VERTEX_PC::FVF, D3DPOOL_MANAGED, &pVb, NULL);
//
//    VERTEX_PC* pVertex;
//    pVb->Lock(0, 0, (LPVOID*)&pVertex, 0);
//    memcpy(pVertex, &vecVertex[0], vecVertex.size() * sizeof(VERTEX_PC));
//    pVb->Unlock();
//    vecVertex.clear();//다 넣어주었으니 필요없다.
//
//    DX::GetDevice()->CreateIndexBuffer(vecIndex.size() * sizeof(WORD), NULL, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIb, NULL);
//    WORD* pIndex;
//    pIb->Lock(0, 0, (LPVOID*)&pIndex, 0);
//    memcpy(pIndex, &vecIndex[0], vecIndex.size() * sizeof(WORD));
//    pIb->Unlock();
//    vecIndex.clear();
//}
//BoxCollider::BoxCollider()
//{
//    D3DXVECTOR3(0, 0, 0);
//    D3DXVECTOR3(5, 5, 5);
//}
//
//BoxCollider::~BoxCollider()
//{
//}
//
//void BoxCollider::Init()
//{
//    D3DXVECTOR3 min = m_min;
//    D3DXVECTOR3 max = m_max;
//
//    m_center = (min + max) * 0.5f;
//    m_vExtent = max - m_center;
//
//    m_vertices.resize(8);
//    m_vertices[0] = VERTEX_PC(D3DXVECTOR3(min.x, min.y, min.z), m_color);
//    m_vertices[1] = VERTEX_PC(D3DXVECTOR3(min.x, max.y, min.z), m_color);
//    m_vertices[2] = VERTEX_PC(D3DXVECTOR3(max.x, max.y, min.z), m_color);
//    m_vertices[3] = VERTEX_PC(D3DXVECTOR3(max.x, min.y, min.z), m_color);
//    m_vertices[4] = VERTEX_PC(D3DXVECTOR3(min.x, min.y, max.z), m_color);
//    m_vertices[5] = VERTEX_PC(D3DXVECTOR3(min.x, max.y, max.z), m_color);
//    m_vertices[6] = VERTEX_PC(D3DXVECTOR3(max.x, max.y, max.z), m_color);
//    m_vertices[7] = VERTEX_PC(D3DXVECTOR3(max.x, min.y, max.z), m_color);
//}
//
//void BoxCollider::Update()
//{
//}
//void BoxCollider::Render()
//{
//    if (!m_isRender) return;
//
//    const auto pD = DX::GetDevice();
//    pD->SetRenderState(D3DRS_LIGHTING, false);
//    pD->SetTransform(D3DTS_WORLD, &m_matWorld);
//    pD->SetTexture(0, nullptr);
//    pD->SetFVF(VERTEX_PC::FVF);
//    pD->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, m_vertices.size(),
//        m_vertices.size() / 2, &m_vertices[0], D3DFMT_INDEX16,
//        &m_vertices[0], sizeof VERTEX_PC);
//}


