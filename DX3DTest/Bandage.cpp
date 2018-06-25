#include "stdafx.h"
#include "Bandage.h"


Bandage::Bandage()
{
    m_pEffectMesh = NULL;
}

Bandage::~Bandage()
{
}

void Bandage::Init()
{
    m_pEffectMesh = g_pResourceManager->GetEffectMesh("./Resource/Bandage/", "Bandage.X");
}

void Bandage::Update()
{
}

void Bandage::Render()
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
