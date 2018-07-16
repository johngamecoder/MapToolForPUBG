#pragma once
#include "ImGuizmo.h"
#define zn_defineinCameraHeader 1
#define zf_defineinCameraHeader 100000.0f
class Camera
{
 private:
	static Camera*	instance;

public:
	D3DXVECTOR3		m_eye;
	
    D3DXVECTOR3     m_moveDir;

    D3DXVECTOR3		m_SavedLookAt;
	D3DXVECTOR3		m_up;
	D3DXMATRIXA16	m_matView;
	D3DXMATRIXA16	m_matProj;

    D3DXMATRIXA16   m_matRot;

    ImGuizmo::OPERATION* m_operation;

    float           m_basePosX;
	float			m_basePosY;
	float			m_rotY;
	float			m_rotX;
    float           m_fov;      //perspective
    float			m_distance; //perspective

    float           m_viewHeight; //orthographic
    float           m_viewWidth;  //orthographic
	
    D3DXVECTOR3*	m_pTarget;
    D3DXVECTOR3    m_pCurrObjPos;

public:
    RECT m_rect;    //client rect
    bool isPerspective;
    bool isHandle;

	Camera();
	~Camera();

public:
	static Camera* Get();
	static void Delete();


    //이건 x 로 facter 값으로 해결을 하자
    D3DXVECTOR3 getLeft()
    {
        D3DXVECTOR3 leftDir = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
        D3DXVec3TransformCoord(&leftDir, &leftDir, &m_matRot);
        D3DXVec3Normalize(&leftDir, &leftDir);
        return leftDir;
    }

    D3DXVECTOR3 getRight()
    {
        D3DXVECTOR3 rightDir = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
        D3DXVec3TransformCoord(&rightDir, &rightDir, &m_matRot);
        D3DXVec3Normalize(&rightDir, &rightDir);
        return rightDir;
    }

    //이건 Y로 해결을 하고 
    D3DXVECTOR3 getUp()
    {
        D3DXVECTOR3 upDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        D3DXVec3TransformCoord(&upDir, &upDir, &m_matRot);
        D3DXVec3Normalize(&upDir, &upDir);
        return upDir;
    }
    D3DXVECTOR3 getDown()
    {
        D3DXVECTOR3 downDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
        D3DXVec3TransformCoord(&downDir, &downDir, &m_matRot);
        D3DXVec3Normalize(&downDir, &downDir);
        return downDir;
    }


	void Init();
	void Update();
	void SetTarget(D3DXVECTOR3* pTarget) { m_pTarget = pTarget; }
    void SetGizmoOperation(ImGuizmo::OPERATION* operation)
    {
        m_operation = operation;
    }
    void SetCurrentObjectPos(D3DXVECTOR3 curObjPos)
    {
        m_pCurrObjPos = curObjPos;
    }
    void PressFtoMovetoTarget() { 
        if(m_pCurrObjPos)
            m_SavedLookAt = m_pCurrObjPos;
    }
	const D3DXVECTOR3& GetPosition() { return m_eye; }
	D3DXMATRIXA16* GetViewMatrix() { return &m_matView; }
	D3DXMATRIXA16* GetProjMatrix() { return &m_matProj; }
	D3DXMATRIXA16* GetViewProjMatrix() { return &(D3DXMATRIXA16)(m_matView * m_matProj); }

};

