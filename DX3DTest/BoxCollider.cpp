#include "stdafx.h"
#include "BoxCollider.h"
#define CUBE_INDEX_SIZE 36
#define CUBE_VERTEX_SIZE 8


BoxCollider::BoxCollider()
{
}

BoxCollider::~BoxCollider()
{
    SAFE_RELEASE(m_pVB);
    SAFE_RELEASE(m_pIB);
}

void BoxCollider::Init()
{
    m_min = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
    m_max = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
    D3DXVECTOR3 g_aCubeVertex[8] = {
        D3DXVECTOR3(m_min.x, m_min.y, m_min.z),
        D3DXVECTOR3(m_min.x, m_max.y, m_min.z),
        D3DXVECTOR3(m_max.x, m_max.y, m_min.z),
        D3DXVECTOR3(m_max.x, m_min.y, m_min.z),
        D3DXVECTOR3(m_min.x, m_min.y, m_max.z),
        D3DXVECTOR3(m_min.x, m_max.y, m_max.z),
        D3DXVECTOR3(m_max.x, m_max.y, m_max.z),
        D3DXVECTOR3(m_max.x, m_min.y, m_max.z)
    };

    vector<D3DXVECTOR3> vecPos;//8개의 cube정점
    for (size_t i = 0; i < CUBE_VERTEX_SIZE; i++)
    {
        vecPos.push_back(g_aCubeVertex[i]);
    }
    //SetVertex(m_vecVertex);
    /*SetVertex(m_vecVertex, vecPos);return;*/
    SetVertex(m_vecVertex, m_vecIndex, vecPos);
    SetBuffer(m_pVB, m_pIB, m_vecVertex, m_vecIndex);

    m_pVB->GetDesc(&m_VBDesc);
    m_pIB->GetDesc(&m_IBDesc);
}

void BoxCollider::Update()
{
    //if (Keyboard::Get()->KeyUp(VK_SPACE))
    //{
    //    D3DXMatrixTranslation(&m_matWorld, ++test, 0, 0);
    //}
}

void BoxCollider::Render()
{
    const auto pD = DX::GetDevice();
    pD->SetRenderState(D3DRS_LIGHTING, false);
    pD->SetTransform(D3DTS_WORLD, &m_matWorld);
    pD->SetFVF(VERTEX_PC::FVF);
    //g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
    //	m_vecVertex.size() / 3,
    //	&m_vecVertex[0], sizeof(VERTEX_PC));
    //g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_vecVertex.size(), m_vecIndex.size() / 3,&m_vecIndex[0],D3DFMT_INDEX16,&m_vecVertex[0],sizeof(VERTEX_PC));
    pD->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PC));
    pD->SetIndices(m_pIB);
    pD->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_VBDesc.Size, 0, m_IBDesc.Size / 3);
}

void BoxCollider::SetVertex(vector<VERTEX_PC>& vecVertexOut, vector<WORD>& vecIndexOut, vector<D3DXVECTOR3> vecPos)
{
    WORD g_aCubeIndex[36] = {
        0, 1, 2, 0, 2, 3,	// 후
        7, 6, 5, 7, 5, 4,	// 전
        4, 5, 1, 4, 1, 0,	// 좌
        3, 2, 6, 3, 6, 7,	// 우
        1, 5, 6, 1, 6, 2,	// 상
        4, 0, 3, 4, 3, 7	// 하
    };
    D3DCOLOR green = D3DCOLOR_XRGB(0, 255, 0);

    vecVertexOut.push_back(VERTEX_PC(vecPos[0], green));
    vecVertexOut.push_back(VERTEX_PC(vecPos[1], green));
    vecVertexOut.push_back(VERTEX_PC(vecPos[2], green));
    vecVertexOut.push_back(VERTEX_PC(vecPos[3], green));
    vecVertexOut.push_back(VERTEX_PC(vecPos[4], green));
    vecVertexOut.push_back(VERTEX_PC(vecPos[5], green));
    vecVertexOut.push_back(VERTEX_PC(vecPos[6], green));
    vecVertexOut.push_back(VERTEX_PC(vecPos[7], green));

    for (size_t i = 0; i < CUBE_INDEX_SIZE; i++)
    {
        vecIndexOut.push_back(g_aCubeIndex[i]);
    }
}

void BoxCollider::SetBuffer(LPDIRECT3DVERTEXBUFFER9 & pVb, LPDIRECT3DINDEXBUFFER9 & pIb, vector<VERTEX_PC>& vecVertex, vector<WORD>& vecIndex)
{																						 //다랙이 pool관리 해주세요
    DX::GetDevice()->CreateVertexBuffer(vecVertex.size() * sizeof(VERTEX_PC), 0, VERTEX_PC::FVF, D3DPOOL_MANAGED, &pVb, NULL);

    VERTEX_PC* pVertex;
    pVb->Lock(0, 0, (LPVOID*)&pVertex, 0);
    memcpy(pVertex, &vecVertex[0], vecVertex.size() * sizeof(VERTEX_PC));
    pVb->Unlock();
    vecVertex.clear();//다 넣어주었으니 필요없다.

    DX::GetDevice()->CreateIndexBuffer(vecIndex.size() * sizeof(WORD), NULL, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIb, NULL);
    WORD* pIndex;
    pIb->Lock(0, 0, (LPVOID*)&pIndex, 0);
    memcpy(pIndex, &vecIndex[0], vecIndex.size() * sizeof(WORD));
    pIb->Unlock();
    vecIndex.clear();
}
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


