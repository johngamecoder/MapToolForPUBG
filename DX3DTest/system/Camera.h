#pragma once
#define zn_defineinCameraHeader 1
#define zf_defineinCameraHeader 1000
class Camera
{
 private:
	static Camera*	instance;

public:
	D3DXVECTOR3		m_eye;
	D3DXVECTOR3		m_lookAt;
    D3DXVECTOR3		m_SavedLookAt;
	D3DXVECTOR3		m_up;
	D3DXMATRIXA16	m_matView;
	D3DXMATRIXA16	m_matProj;

    float           m_basePosX;
	float			m_basePosY;
	float			m_rotY;
	float			m_rotX;
    float           m_fov;      //perspective
    float			m_distance; //perspective

    float           m_viewHeight; //orthographic
    float           m_viewWidth;  //orthographic
	
    D3DXVECTOR3*	m_pTarget;

public:
    RECT m_rect;    //client rect
    bool isPerspective;
    bool isHandle;

	Camera();
	~Camera();

public:
	static Camera* Get();
	static void Delete();

	void Init();
	void Update();
	void SetTarget(D3DXVECTOR3* pTarget) { m_pTarget = pTarget; }
	const D3DXVECTOR3& GetPosition() { return m_eye; }
	D3DXMATRIXA16* GetViewMatrix() { return &m_matView; }
	D3DXMATRIXA16* GetProjMatrix() { return &m_matProj; }
	D3DXMATRIXA16* GetViewProjMatrix() { return &(D3DXMATRIXA16)(m_matView * m_matProj); }


};

