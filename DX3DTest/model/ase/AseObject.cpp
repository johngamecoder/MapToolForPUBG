#include "stdafx.h"
#include "AseObject.h"

AseObject::AseObject()
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
}


AseObject::~AseObject()
{
	
}

void AseObject::Init()
{
	CalcLocalTM();
	SetupTexture();
}


void AseObject::Update(int tickCount, D3DXMATRIXA16 * pMatParent)
{
	D3DXMATRIXA16 matLocalT, matLocalR;

	CalcLocalTranslationMatrix(tickCount, matLocalT);
	CalcLocalRotationMatrix(tickCount, matLocalR);

	m_matLocalTM = matLocalR * matLocalT;
	
	m_matWorldTM = m_matLocalTM;

	if (pMatParent)
		m_matWorldTM *= *pMatParent;

	for each(auto p in m_vecChild)
	{
		p->Update(tickCount, &m_matWorldTM);
	}
}

void AseObject::Render()
{
	//쉐이더 지정
	LPD3DXEFFECT m_pEffect = NULL;
	m_pEffect = ShaderManager::Get()->GetShader(SHADER_PATH + _T("Lighting.fx"));

	//광원 위치 업데이트
	static float angle = -D3DX_PI / 6;
	//angle += 0.02f * 0.1f;
	float radius = 500;
	D3DXVECTOR4 lightPos(cosf(angle) * radius, radius, sinf(angle) * radius, 1);
	m_pEffect->SetVector("gWorldLightPosition", &lightPos);

	//카메라 위치 업데이트
	D3DXVECTOR4 camPos(Camera::Get()->GetPosition(), 1);
	m_pEffect->SetVector("gWorldCameraPosition", &camPos);

	//매트릭스 업데이트
	m_pEffect->SetMatrix("gWorldMatrix", &m_matWorldTM);
	m_pEffect->SetMatrix("gViewMatrix", Camera::Get()->GetViewMatrix());
	m_pEffect->SetMatrix("gProjMatrix", Camera::Get()->GetProjMatrix());

	m_pEffect->SetTexture("DiffuseMap_Tex", m_pTex);

	LPDIRECT3DVERTEXSHADER9 vs;
	LPDIRECT3DPIXELSHADER9 ps;

	D3DVERTEXELEMENT9 vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(VERTEX_PNT::FVF, vertexDecl);

	D3DVERTEXELEMENT9 elements[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	LPDIRECT3DVERTEXDECLARATION9    pDecl = NULL;
	DX::GetDevice()->CreateVertexDeclaration(vertexDecl, &pDecl);
	DX::GetDevice()->SetVertexDeclaration(pDecl);
	//DX::GetDevice()->SetFVF(VERTEX_PNT::FVF);
	//m_pEffect->SetTechnique("Lighting");
	
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);

	for (int i = 0; i < numPasses; i++)
	{
		m_pEffect->BeginPass(i);

		DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(VERTEX_PNT));

		m_pEffect->EndPass();
	}

	m_pEffect->End();
	SAFE_RELEASE(pDecl);
	

	//D3DXMatrixIdentity(&m_matWorldTM);
	//DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorldTM);
	//DX::GetDevice()->SetTexture(0, m_pTex );
	//m_mtl.Diffuse.a = 0.5f;
	//DX::GetDevice()->SetMaterial(&m_mtl);
	//DX::GetDevice()->SetFVF(VERTEX_PNT::FVF);
	//DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(VERTEX_PNT));

	for each(auto p in m_vecChild)
	{
		p->Render();
	}
}

void AseObject::AddChild(AseObject * pChild)
{
	if (pChild == nullptr)
		return;

	m_vecChild.push_back(pChild);
}

void AseObject::ReleaseAll()
{
	for (auto child : m_vecChild)
	{
		child->ReleaseAll();
	}
  
	Release();
}



void AseObject::SetupTexture()
{
	m_pTex = TextureManager::Get()->GetTexture(m_texName);

	for each(auto p in m_vecChild)
	{
		p->SetupTexture();
	}
}


void AseObject::CalcLocalTM(AseObject * pParent)
{
	m_matLocalTM = m_matWorldTM;

	if (pParent)
	{
		D3DXMATRIXA16 matInvParent;
		D3DXMatrixInverse(&matInvParent, 0, &pParent->m_matWorldTM);
		m_matLocalTM *= matInvParent;
	}

	for each(auto p in m_vecChild)
	{
		p->CalcLocalTM(this);
	}
}


void AseObject::CalcLocalTranslationMatrix(IN int tickCount, OUT D3DXMATRIXA16 & mat)
{
  D3DXMatrixIdentity(&mat);
  if (m_vecPosTrack.empty())	//Track 정보 없을 때 Local 로 채워줌
  {
    mat._41 = m_matLocalTM._41;
    mat._42 = m_matLocalTM._42;
    mat._43 = m_matLocalTM._43;
  }
  else if (tickCount <= m_vecPosTrack.front().tick)
  {
    mat._41 = m_vecPosTrack.front().position.x;
    mat._42 = m_vecPosTrack.front().position.y;
    mat._43 = m_vecPosTrack.front().position.z;
  }
  else if (tickCount >= m_vecPosTrack.back().tick)
  {
    mat._41 = m_vecPosTrack.back().position.x;
    mat._42 = m_vecPosTrack.back().position.y;
    mat._43 = m_vecPosTrack.back().position.z;
  }
  else
  {
    int nNext = -1;
    for (size_t i = 0; i < m_vecPosTrack.size(); ++i)
    {
      if (m_vecPosTrack[i].tick > tickCount)
      {
        nNext = i;
        break;
      }
    }
    int nPrev = nNext - 1;

    float t = (tickCount - m_vecPosTrack[nPrev].tick) /
      (float)(m_vecPosTrack[nNext].tick - m_vecPosTrack[nPrev].tick);

    D3DXVECTOR3 v;
    D3DXVec3Lerp(&v, &m_vecPosTrack[nPrev].position, &m_vecPosTrack[nNext].position, t);
    mat._41 = v.x;
    mat._42 = v.y;
    mat._43 = v.z;
  }
}

void AseObject::CalcLocalRotationMatrix(IN int tickCount, OUT D3DXMATRIXA16 & mat)
{
  D3DXMatrixIdentity(&mat);
  if (m_vecRotTrack.empty())	//Track 정보 없을 때 Local 로 채워줌
  {
    mat = m_matLocalTM;
    mat._41 = 0.0f;
    mat._42 = 0.0f;
    mat._43 = 0.0f;
  }
  else if (tickCount <= m_vecRotTrack.front().tick)
  {
    D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.front().quaternion);
  }
  else if (tickCount >= m_vecRotTrack.back().tick)
  {
    D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.back().quaternion);
  }
  else
  {
    int nNext = -1;
    for (size_t i = 0; i < m_vecRotTrack.size(); ++i)
    {
      if (m_vecRotTrack[i].tick > tickCount)
      {
        nNext = i;
        break;
      }
    }
    int nPrev = nNext - 1;

    float t = (tickCount - m_vecRotTrack[nPrev].tick) /
      (float)(m_vecRotTrack[nNext].tick - m_vecRotTrack[nPrev].tick);
	
    D3DXQUATERNION q;
    D3DXQuaternionSlerp(&q, &m_vecRotTrack[nPrev].quaternion, &m_vecRotTrack[nNext].quaternion, t);
    D3DXMatrixRotationQuaternion(&mat, &q);
  }
}



