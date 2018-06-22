#pragma once
#include "IDisplayObject.h"

class IUnitObject : public IDisplayObject
{
protected:
	D3DXVECTOR3		m_deltaPos;
	D3DXVECTOR3		m_deltaRot;
	D3DXVECTOR3		m_forward;
	D3DXVECTOR3		m_baseRotation;

	bool			m_isMoving;
	float			m_moveSpeed;
	float			m_currMoveSpeedRate;
	float			m_rotationSpeed;

	bool			m_isJumping;
	float			m_jumpPower;
	float			m_gravity;
	float			m_currGravity;

	float			m_maxStepHeight;

	D3DXVECTOR3		m_destPos;
	D3DXVECTOR3		m_finalDestPos;
	vector<int>		m_vecAStarIndex;

	IUnitObject();

public:
	
	virtual ~IUnitObject();
	
	void SetDestination(const D3DXVECTOR3 &pos);
	void UpdateKeyboardState();
	void UpdatePositionToDestination();
	void UpdateTargetPosition(OUT D3DXVECTOR3 &targetPos);
	void ApplyTargetPosition(D3DXVECTOR3 &targetPos);
	
	void UpdatePosition();
	
};

