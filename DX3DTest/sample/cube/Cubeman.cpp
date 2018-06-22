#include "stdafx.h"
#include "Cubeman.h"
#include "CubemanParts.h"


Cubeman::Cubeman()
{
	m_pRootCube = NULL;
	m_isTurnedOnLight = false;

	m_isMoving = false;
	m_moveSpeed = 0.2f;
	m_currMoveSpeedRate = 1.0f;
	m_rotationSpeed = 0.1f;

	m_isJumping = false;
	m_jumpPower = 1.0f;
	m_gravity = 0.05f;
	m_currGravity = 0.0f;

	m_maxStepHeight = 0.5f;
}


Cubeman::~Cubeman()
{
}

void Cubeman::Init()
{
	Camera::Get()->SetTarget(&m_pos);
	
	CreateAllParts();
}

void Cubeman::Update()
{
	UpdatePosition();

	if (GetAsyncKeyState('1') & 0x0001)
	{
		m_isTurnedOnLight = !m_isTurnedOnLight;
	}
	
	if (m_isTurnedOnLight == true)
	{
		D3DXVECTOR3 pos = m_pos;
		pos.x += m_forward.x * 0.5f;
		pos.z += m_forward.z * 0.5f;
		pos.y += 1.5f;

		D3DLIGHT9 light = DXUtil::InitSpot(&m_forward, &pos, &WHITE);
		light.Range = 35;
		light.Phi = D3DX_PI / 4;
		light.Theta = D3DX_PI / 6;
		light.Falloff = 1.0f;
		DX::GetDevice()->SetLight(10, &light);
	}
	DX::GetDevice()->LightEnable(10, m_isTurnedOnLight);
	
	m_pRootCube->SetMovingState(m_isMoving);
	m_pRootCube->Update();
}

void Cubeman::Render()
{
	m_pRootCube->Render();
}


void Cubeman::UpdatePosition()
{
	if (Keyboard::Get()->KeyPress('W')) m_deltaPos.z = 1;
	else if (Keyboard::Get()->KeyPress('S')) m_deltaPos.z = -1;
	else m_deltaPos.z = 0;

	if (Keyboard::Get()->KeyPress('D')) m_deltaRot.y = 1;
	else if (Keyboard::Get()->KeyPress('A')) m_deltaRot.y = -1;
	else m_deltaRot.y = 0;

	if (Keyboard::Get()->KeyDown(VK_SPACE) && m_isJumping == false) m_isJumping = true;

	m_rot += m_deltaRot * m_rotationSpeed;

	D3DXMATRIXA16 matRotY;
	D3DXMatrixRotationY(&matRotY, m_rot.y);
	D3DXVec3TransformNormal(&m_forward, &D3DXVECTOR3(0, 0, 1), &matRotY);

	D3DXVECTOR3 targetPos;
	//float basePosY = 0;
	float height = 0;
	bool isIntersected = true;

	if (m_isJumping == true)
	{
		m_currMoveSpeedRate = 0.7f;
		targetPos = m_pos + m_forward * m_deltaPos.z * m_moveSpeed * m_currMoveSpeedRate;

		targetPos.y += m_jumpPower - m_currGravity;
		m_currGravity += m_gravity;

		if (g_pCurrentMap != NULL)
		{
			isIntersected = g_pCurrentMap->GetHeight(OUT height, targetPos);
		}

		if (isIntersected == false)
		{
			//이동할 위치에 바닥이 없으면 현재 x, z 의 height 갱신
			//m_pos.y 값만 반영
			if (g_pCurrentMap != NULL)
			{
				isIntersected = g_pCurrentMap->GetHeight(OUT height, m_pos);
			}

			m_pos.y = targetPos.y;
		}
		else
		{
			m_pos = targetPos;
		}
		//하강중 m_pos.y 가 height 보다 낮으면 height 로 고정. 점프 종료
		if (m_pos.y <= height && m_jumpPower < m_currGravity)
		{
			m_pos.y = height;
			m_isJumping = false;
			m_currGravity = 0;
			m_currMoveSpeedRate = 1.0f;
		}

		//if (targetPos.y <= basePosY)
		if (targetPos.y <= height)
		{
			//targetPos.y = basePosY;
			targetPos.y = height;
			m_isJumping = false;
			m_currGravity = 0;
			m_currMoveSpeedRate = 1.0f;
		}

		//m_pos = targetPos;
	}
	else //m_isJumping == false
	{
		targetPos = m_pos + m_forward * m_deltaPos.z * m_moveSpeed * m_currMoveSpeedRate;
		
		if (g_pCurrentMap != NULL)
		{
			isIntersected = g_pCurrentMap->GetHeight(OUT height, targetPos);
			//이동할 위치에 바닥이 없거나 높이차가 크면 이동 막는다
			if (isIntersected == false || fabs(height - m_pos.y) > m_maxStepHeight)
			{
			}
			else
			{
				targetPos.y = height;
				m_pos = targetPos;
			}
		}
		else
		{
			m_pos = targetPos;
		}

		//m_pos = targetPos;
	}

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_pos.x, m_pos.y, m_pos.z);
	m_matWorld = matRotY * matT;

	if (D3DXVec3LengthSq(&m_deltaRot) > D3DX_16F_EPSILON || D3DXVec3LengthSq(&m_deltaPos) > D3DX_16F_EPSILON)
		m_isMoving = true;
	else
		m_isMoving = false;
}


void Cubeman::CreateAllParts()
{
	CubemanParts* pParts;
	// 몸통
	m_pRootCube = new CubemanParts();
	CreateParts(m_pRootCube, this, D3DXVECTOR3(0.0f, 3.0f, 0.0f),
		D3DXVECTOR3(1.0f, 1.0f, 0.5f), D3DXVECTOR3(0, 0, 0), uvBody);
	// 머리
	pParts = new CubemanParts();
	CreateParts(pParts, m_pRootCube, D3DXVECTOR3(0.0f, 1.6f, 0.0f),
		D3DXVECTOR3(0.8f, 0.8f, 0.8f), D3DXVECTOR3(0, 0, 0), uvHead);
	// 왼팔
	pParts = new CubemanParts(0.1f);
	CreateParts(pParts, m_pRootCube, D3DXVECTOR3(-1.5f, 1.0f, 0.0f),
		D3DXVECTOR3(0.5f, 1.0f, 0.5f), D3DXVECTOR3(0, -1.0f, 0), uvLArm);
	// 오른팔
	pParts = new CubemanParts(-0.1f);
	CreateParts(pParts, m_pRootCube, D3DXVECTOR3(1.5f, 1.0f, 0.0f),
		D3DXVECTOR3(0.5f, 1.0f, 0.5f), D3DXVECTOR3(0, -1.0f, 0), uvRArm);
	// 왼다리
	pParts = new CubemanParts(-0.1f);
	CreateParts(pParts, m_pRootCube, D3DXVECTOR3(-0.5f, -1.0f, 0.0f),
		D3DXVECTOR3(0.5f, 1.0f, 0.5f), D3DXVECTOR3(0, -1.0f, 0), uvLLeg);
	// 오른다리
	pParts = new CubemanParts(0.1f);
	CreateParts(pParts, m_pRootCube, D3DXVECTOR3(0.5f, -1.0f, 0.0f),
		D3DXVECTOR3(0.5f, 1.0f, 0.5f), D3DXVECTOR3(0, -1.0f, 0), uvRLeg);
}


void Cubeman::CreateParts(CubemanParts* &pParts, IDisplayObject* pParent, D3DXVECTOR3 pos,
	D3DXVECTOR3 scale, D3DXVECTOR3 trans, vector<vector<int>> &vecUV)
{
	D3DXMATRIXA16 matS, matT, mat;
	D3DXMatrixScaling(&matS, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&matT, trans.x, trans.y, trans.z);
	mat = matS * matT;
	pParts->Init(&mat, vecUV);
	pParts->SetPosition(&pos);
	pParent->AddChild(pParts);
}
