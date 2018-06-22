#include "stdafx.h"
#include "Church.h"


Church::Church()
{
    m_pEffectMesh = NULL;
}


Church::~Church()
{
    //SAFE_DELETE(m_pEffectMesh); //이친구가 release 해주는 것이 아니라 resourcemanager에서 해준다
}

void Church::Init()
{
    m_pEffectMesh = g_pResourceManager->GetEffectMesh("./Resource/Church/", "Church.X");
}

void Church::Update()
{
}
void Church::Render()
{
    for (size_t i = 0u; i < m_pEffectMesh->EffectParams.size(); ++i)
    {
        const auto& ep = m_pEffectMesh->EffectParams[i];
        ep.pEffect->ApplyParameterBlock(ep.hParam);

        ep.pEffect->SetMatrix("World", &m_matWorld);
        ep.pEffect->SetMatrix("View", Camera::Get()->GetViewMatrix());
        ep.pEffect->SetMatrix("Projection", Camera::Get()->GetProjMatrix());

        UINT numPasses;
        ep.pEffect->Begin(&numPasses, 0);
        for (UINT p = 0u; p < numPasses; ++p)
        {
            ep.pEffect->BeginPass(p);
            m_pEffectMesh->pMesh->DrawSubset(i);
            ep.pEffect->EndPass();
        }
        ep.pEffect->End();
    }
}
//void Church::Render()
//{
//    for (size_t i = 0u; i < m_pEffectMesh->EffectParams.size(); ++i)
//    {
//        const auto& ep = m_pEffectMesh->EffectParams[i];
//        ep.pEffect->ApplyParameterBlock(ep.hParam);
//
//        //D3DXMATRIX world;
//        //if (auto tr = GetOwnerTransform())
//        //{
//        //    world = tr->GetTransformationMatrix();
//        //}
//        //else
//        //{
//        //    D3DXMatrixIdentity(&world);
//        //}
//        //ep.pEffect->SetMatrix("World", &world);
//        ep.pEffect->SetMatrix("World", &m_matWorld);
//
//        D3DXMATRIX view;
//        //ep.pEffect->SetMatrix("View", &g_pCurrentCamera->GetViewMatrix());
//        ep.pEffect->SetMatrix("View", Camera::Get()->GetViewMatrix());
//
//        D3DXMATRIX proj;
//        //ep.pEffect->SetMatrix("Projection", &g_pCurrentCamera->GetProjectionMatrix());
//        ep.pEffect->SetMatrix("Projection", Camera::Get()->GetProjMatrix());
//
//        //D3DXVECTOR3 lightDirection(1.0f, -1.0f, 1.0f);
//        //D3DXVec3Normalize(&lightDirection, &lightDirection);
//        //ep.pEffect->SetValue("lightDirection", &lightDirection, sizeof lightDirection);
//
//        UINT numPasses;
//        ep.pEffect->Begin(&numPasses, 0);
//        for (UINT p = 0u; p < numPasses; ++p)
//        {
//            ep.pEffect->BeginPass(p);
//            m_pEffectMesh->pMesh->DrawSubset(i);
//            ep.pEffect->EndPass();
//        }
//        ep.pEffect->End();
//    }
//}
