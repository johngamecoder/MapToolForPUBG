#include "stdafx.h"
#include "AseUnit.h"
#include "AseObject.h"
#include "AseLoader.h"

AseUnit::AseUnit()
{
	m_pCurrentObj = NULL;
	m_pStandObj = NULL;
	m_pRunObj = NULL;
	m_moveSpeed = 0.2f;
	m_rotationSpeed = 0.1f;
	
	m_jumpPower = 0.4f;
	m_gravity = 0.025f;
	m_baseRotation.y = D3DX_PI;

	m_pos.y = 3;
}


AseUnit::~AseUnit()
{
	if (m_pStandObj)
		m_pStandObj->ReleaseAll();

	if (m_pRunObj)
		m_pRunObj->ReleaseAll();
}

void AseUnit::Init()
{
	Camera::Get()->SetTarget(&m_pos);
	ObjectManager::Get()->AddToTagList(TAG_PLAYER, this);

	CString path = ASSET_PATH + _T("models/ase/");
	CString name = _T("woman_01_all_stand.ASE");
	Load(m_pStandObj, path, name);
	m_pStandObj->Init();

	name = _T("woman_01_all.ASE");
	Load(m_pRunObj, path, name);
	m_pRunObj->Init();

	SetCurrentObject(m_pStandObj);

	ParticleSystem* pAura = new Aura(ASSET_PATH + _T("Textures/particle/particle1.png"),
		&m_pos, 2.0f);
	pAura->Init();
	ObjectManager::Get()->AddParticle(pAura);
}


void AseUnit::Update()
{	
	//IUnitObject::UpdatePosition();
	IUnitObject::UpdateKeyboardState();
	IUnitObject::UpdatePositionToDestination();

	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 2, 2, 2);
	//D3DXMatrixIdentity(&matS);
	m_matWorld = matS * m_matWorld;

	if (m_isMoving == true)
	{
		if (m_pCurrentObj != m_pRunObj)
			SetCurrentObject(m_pRunObj);
	}
	else if (m_isMoving == false)
	{
		if (m_pCurrentObj != m_pStandObj)
			SetCurrentObject(m_pStandObj);
	}
	
	int nFirstTick = m_aseScene.numFirstFrame * m_aseScene.TicksPerFrame;
	int nLastTick = m_aseScene.numLastFrame * m_aseScene.TicksPerFrame;
	int nTickSpeed = m_aseScene.FrameSpeed * m_aseScene.TicksPerFrame;
	DWORD tickCount = ((int)((nTickSpeed / 1000.f) * GetTickCount()) % (nLastTick - nFirstTick) + nFirstTick);

	if (m_pCurrentObj)
		m_pCurrentObj->Update(tickCount, &m_matWorld);

}

void AseUnit::Render()
{
	//PreRender
	//DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	DX::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DX::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//D3DBLEND_ZERO		D3DBLEND_ONE	D3DBLEND_BLENDFACTOR
	//D3DBLEND_SRCALPHA	 D3DBLEND_INVSRCALPHA	D3DBLEND_SRCCOLOR	D3DBLEND_INVSRCCOLOR
	//D3DBLEND_DESTCOLOR
	DX::GetDevice()->SetRenderState(D3DRS_BLENDFACTOR, 0xffff22cc);
	
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	
	//D3DTOP_ADD	D3DTOP_DISABLE	D3DTOP_MODULATE	
	//D3DTOP_SUBTRACT	D3DTOP_SELECTARG1

	//D3DTA_DIFFUSE	 D3DTA_CURRENT	 D3DTA_SPECULAR

	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	SAFE_RENDER(m_pCurrentObj);

	//PostRender
	DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

}


void AseUnit::Load(OUT AseObject* &rootObj, LPCTSTR filepath, LPCTSTR filename)
{
	AseLoader pAseLoader;
	rootObj = pAseLoader.Load(filepath, filename, &m_aseScene);
}

void AseUnit::SetCurrentObject(AseObject * pObj)
{
	m_pCurrentObj = pObj;
}
