#include "stdafx.h"
#include "ColorShader.h"

ColorShader::ColorShader()
{
}

ColorShader::~ColorShader()
{
	SAFE_RELEASE(m_pMesh);
}


void ColorShader::Init()
{
	//쉐이더 지정
	m_pEffect = ShaderManager::Get()->GetShader(SHADER_PATH + _T("ColorShader.fx"));

	//모델 지정
	D3DXCreateSphere(DX::GetDevice(), 2, 10, 10, &m_pMesh, NULL);
	
	//매트릭스 업데이트
	m_pEffect->SetMatrix("gProjMatrix", Camera::Get()->GetProjMatrix());
}

void ColorShader::Update()
{
	static float rad = 0;
	rad += 0.02f;
	m_pEffect->SetFloat("gConst", sinf(rad) * 1.0f);

	//매트릭스 업데이트
	m_pEffect->SetMatrix("gWorldMatrix", &m_matWorld);
	m_pEffect->SetMatrix("gViewMatrix", Camera::Get()->GetViewMatrix());
}

void ColorShader::Render()
{
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);

	for (int i = 0; i < numPasses; i++)
	{
		m_pEffect->BeginPass(i);
		m_pMesh->DrawSubset(0);
		m_pEffect->EndPass();
	}

	m_pEffect->End();
}
