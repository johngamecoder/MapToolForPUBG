#include "stdafx.h"
#include "PUBG_Object.h"

PUBG_Object::PUBG_Object(string& path, string& fileName)
    :m_Path(path),
     m_FileName(fileName)
{
    m_pEffectMesh = NULL;
}

PUBG_Object::~PUBG_Object()
{
}

void PUBG_Object::Init()
{
    m_pEffectMesh = g_pResourceManager->GetEffectMesh(m_Path, m_FileName);
}

void PUBG_Object::Update()
{
}

void PUBG_Object::Render()
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
