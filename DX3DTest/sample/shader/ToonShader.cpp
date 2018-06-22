#include "stdafx.h"
#include "ToonShader.h"

ToonShader::ToonShader()
{
}

ToonShader::~ToonShader()
{
	SAFE_RELEASE(m_pMesh);
}


void ToonShader::Init()
{
	//쉐이더 지정
	m_pEffect = ShaderManager::Get()->GetShader(SHADER_PATH + _T("ToonShader.fx"));
	
	//모델 지정
	D3DXCreateTeapot(DX::GetDevice(), &m_pMesh, NULL);
	
	D3DXVECTOR4 color(0, 1, 0, 0);
	m_pEffect->SetVector("gSurfaceColor", &color);

	//매트릭스 업데이트
	D3DXMATRIXA16 matInvWorld, matVP;
	D3DXMatrixInverse(&matInvWorld, NULL, &m_matWorld);
	m_pEffect->SetMatrix("gInvWMatrix", &matInvWorld);

	matVP = *Camera::Get()->GetViewProjMatrix();
	m_pEffect->SetMatrix("gWVPMatrix", &(m_matWorld * matVP));
	
}

void ToonShader::Update()
{
	//광원 위치 업데이트
	static float angle = -D3DX_PI / 6;
	//angle += 0.02f;
	float radius = 500;
	D3DXVECTOR4 lightPos(cosf(angle) * radius, radius, sinf(angle) * radius, 1);
	m_pEffect->SetVector("gWorldLightPosition", &lightPos);

	//카메라 위치 업데이트
	D3DXVECTOR4 camPos(Camera::Get()->GetPosition(), 1);
	m_pEffect->SetVector("gWorldCameraPosition", &camPos);

	//매트릭스 업데이트
	D3DXMATRIXA16 matInvWorld, matVP;
	D3DXMatrixInverse(&matInvWorld, NULL, &m_matWorld);
	m_pEffect->SetMatrix("gInvWMatrix", &matInvWorld);

	matVP = *Camera::Get()->GetViewProjMatrix();
	m_pEffect->SetMatrix("gWVPMatrix", &(m_matWorld * matVP));
}

void ToonShader::Render()
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