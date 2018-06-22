#include "stdafx.h"
#include "IUnitObject.h"
#include "AStar.h"

IUnitObject::IUnitObject()
{
	m_forward = D3DXVECTOR3(0, 0, 1);
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


IUnitObject::~IUnitObject()
{
}

void IUnitObject::SetDestination(const D3DXVECTOR3 & pos)
{
	//m_destPos = pos; m_finalDestPos = m_destPos; return;

	m_vecAStarIndex.clear();
	m_destPos = m_pos;
	m_finalDestPos = pos;
	g_pCurrentMap->GetAStar()->FindPath(m_pos, pos, m_vecAStarIndex);
	g_pCurrentMap->GetAStar()->MakeDirectPath(m_pos, pos, m_vecAStarIndex);
}


void IUnitObject::UpdateKeyboardState()
{
	if (Keyboard::Get()->KeyPress('W')) m_deltaPos.z = 1;
	else if (Keyboard::Get()->KeyPress('S')) m_deltaPos.z = -1;
	else m_deltaPos.z = 0;

	if (Keyboard::Get()->KeyPress('D')) m_deltaRot.y = 1;
	else if (Keyboard::Get()->KeyPress('A')) m_deltaRot.y = -1;
	else m_deltaRot.y = 0;

	if (Keyboard::Get()->KeyDown(VK_SPACE) && m_isJumping == false) m_isJumping = true;

	if (m_deltaPos.z == 0 && m_deltaRot.y == 0 )
		return;
	
	D3DXMATRIXA16 matRY;
	D3DXMatrixRotationY(&matRY, m_deltaRot.y * m_rotationSpeed);
	D3DXVec3TransformNormal(&m_forward, &m_forward, &matRY);
	
	m_vecAStarIndex.clear();
	m_destPos = (D3DXVECTOR3(m_pos + m_forward * m_deltaPos.z * m_moveSpeed * m_currMoveSpeedRate));
	m_finalDestPos = m_destPos;
}


void IUnitObject::UpdatePositionToDestination()
{
 //   static float x, y, z = 0;
 //   ImGui::SliderFloat("x", &x, 0.0f, 100.0f);
 //   ImGui::SliderFloat("y", &y, 0.0f, 100.0f);
 //   ImGui::SliderFloat("z", &z, 0.0f, 100.0f);
 //   m_pos = D3DXVECTOR3(x, y, z);
	D3DXVECTOR3 targetPos = m_pos;
	
	UpdateTargetPosition(targetPos);
	ApplyTargetPosition(targetPos);
	
	D3DXMATRIXA16 m_matRotY;
	D3DXMatrixLookAtLH(&m_matRotY, &D3DXVECTOR3(0, 0, 0), &m_forward, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&m_matRotY, &m_matRotY);
	D3DXMATRIXA16 matBaseR;
	D3DXMatrixRotationY(&matBaseR, m_baseRotation.y);
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_pos.x, m_pos.y, m_pos.z);
	m_matWorld = m_matRotY * matBaseR * matT;

	if (D3DXVec3LengthSq(&m_deltaPos) > 0 || D3DXVec3LengthSq(&m_deltaRot) > 0)
		m_isMoving = true;
	else
		m_isMoving = false;
}


void IUnitObject::UpdateTargetPosition(OUT D3DXVECTOR3 & targetPos)
{
	D3DXVECTOR3 forward = D3DXVECTOR3(m_destPos.x - m_pos.x, 0, m_destPos.z - m_pos.z);
	D3DXVECTOR3 forwardNormalized = forward;

	if (D3DXVec3LengthSq(&forward) > 0)
	{
		D3DXVec3Normalize(&forwardNormalized, &forwardNormalized);
		m_forward = forwardNormalized;

		if (m_deltaPos.z == 1)
		{
			m_currMoveSpeedRate = 1.0f;
		}
		else if (m_deltaPos.z == -1)
		{
			m_forward *= -1;
			m_currMoveSpeedRate = 0.5f;
		}
		else
		{
			m_deltaPos.z = 1;
			m_currMoveSpeedRate = 1.0f;
		}

		if (D3DXVec3Length(&(forward)) >= m_moveSpeed * m_currMoveSpeedRate)
		{
			targetPos = m_pos + forwardNormalized * m_moveSpeed * m_currMoveSpeedRate;
		}
		else
		{
			targetPos.x = m_destPos.x;
			targetPos.z = m_destPos.z;
		}
	}
	else if (m_vecAStarIndex.empty() == false)
	{
		//목적지 도달 후 Astar 노드가 남아있을 시 목적지 재설정
		m_destPos = g_pCurrentMap->GetAStar()->GetNodes()[m_vecAStarIndex.back()]->GetLocation();
		m_vecAStarIndex.pop_back();
	}
	else
	{
		m_destPos = m_finalDestPos;
	}
}

void IUnitObject::ApplyTargetPosition(D3DXVECTOR3 &targetPos)
{
	float height = 0;
	bool isIntersected = true;

	if (m_isJumping == true)
	{
		m_currMoveSpeedRate = 0.7f;
		//targetPos = m_pos + m_forward * m_deltaPos.z * m_moveSpeed * m_currMoveSpeedRate;

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
	}
	else	//m_isJumping == false
	{
		//targetPos = m_pos + m_forward * m_deltaPos.z * m_moveSpeed * m_currMoveSpeedRate;

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
	}
}

void IUnitObject::UpdatePosition()
{
	if (Keyboard::Get()->KeyPress('W')) m_deltaPos.z = 1;
	else if (Keyboard::Get()->KeyPress('S')) m_deltaPos.z = -1;
	else m_deltaPos.z = 0;

	if (Keyboard::Get()->KeyPress('D')) m_deltaRot.y = 1;
	else if (Keyboard::Get()->KeyPress('A')) m_deltaRot.y = -1;
	else m_deltaRot.y = 0;

	if (Keyboard::Get()->KeyDown(VK_SPACE) && m_isJumping == false ) m_isJumping = true;

	m_rot += m_deltaRot * m_rotationSpeed;
	
	D3DXMATRIXA16 matRotY;
	D3DXMatrixRotationY(&matRotY, m_rot.y);
	D3DXVec3TransformNormal(&m_forward, &D3DXVECTOR3(0, 0, 1), &matRotY);

	D3DXVECTOR3 targetPos;
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
	}
	else	//m_isJumping == false
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
	}

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixRotationY(&matRotY, m_rot.y + m_baseRotation.y);
	m_matWorld = matRotY * matT;

	if (D3DXVec3LengthSq(&m_deltaPos) > 0 || D3DXVec3LengthSq(&m_deltaRot) > 0)
		m_isMoving = true;
	else
		m_isMoving = false;

}

