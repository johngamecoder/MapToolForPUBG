#include "stdAfx.h"
#include "Camera.h"

Camera* Camera::instance = NULL;

Camera * Camera::Get()
{
	if (instance == NULL)
		instance = new Camera();

	return instance;
}

void Camera::Delete()
{
	SAFE_DELETE(instance);
}

Camera::Camera()
{
	m_pTarget = NULL;
	m_distance = 10.0f;
	m_basePosY = 2.0f;
	m_eye = D3DXVECTOR3(0, m_basePosY, -m_distance);
	m_lookAt = D3DXVECTOR3(0, 0, 0);
	m_up = D3DXVECTOR3(0, 1, 0);
	m_rotX = 0.0f;
	m_rotY = 0.0f;
    m_fov = D3DX_PI / 4.0f;

    isPerspective = true;
    m_viewWidth = 10.f;
}


Camera::~Camera()
{
}

void Camera::Init()
{
	GetClientRect(DX::GetInfo().hWnd, &m_rect);

	D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_lookAt, &m_up);
	DX::GetDevice()->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fov,
        m_rect.right / (float)m_rect.bottom, zn_defineinCameraHeader, zf_defineinCameraHeader);
	DX::GetDevice()->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void Camera::Update()
{
    if (ImGui::IsKeyPressed(VK_MENU))
    {
        if (Mouse::Get()->ButtonPress(Mouse::LBUTTON))
        {
            m_rotX += (Mouse::Get()->GetDeltaPosition().y) / 100.f;
            m_rotY += (Mouse::Get()->GetDeltaPosition().x) / 100.f;

            //X축 회전 제한
            if (m_rotX <= -D3DX_PI * 0.4f)
            {
                m_rotX = -D3DX_PI * 0.4f;
            }
            if (m_rotX >= D3DX_PI * 0.35f)
            {
                m_rotX = D3DX_PI * 0.35f;
            }
        }
    }


	m_distance -= Mouse::Get()->GetDeltaPosition().z / 120.0f;
	m_distance = max(2, m_distance);
	m_distance = min(100, m_distance);
	
	m_eye = D3DXVECTOR3(0, m_basePosY, -m_distance);

	D3DXMATRIXA16 matRotX, matRotY, matRot;
	D3DXMatrixRotationX(&matRotX, m_rotX);
	D3DXMatrixRotationY(&matRotY, m_rotY);
	matRot = matRotX * matRotY;
	D3DXVec3TransformCoord(&m_eye, &m_eye, &matRot);
	
	if (m_pTarget)
	{
		m_lookAt = *m_pTarget;
		m_eye = *m_pTarget + m_eye;
	}
	
	D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_lookAt, &m_up);
	DX::GetDevice()->SetTransform(D3DTS_VIEW, &m_matView);

    if (isPerspective)
    {
        D3DXMatrixPerspectiveFovLH(&m_matProj, m_fov,
            m_rect.right / (float)m_rect.bottom, zn_defineinCameraHeader, zf_defineinCameraHeader);
        DX::GetDevice()->SetTransform(D3DTS_PROJECTION, &m_matProj);
    }
    else //when it it orthographic
    {
        D3DXMatrixOrthoLH(&m_matProj, m_viewWidth, m_viewHeight, zn_defineinCameraHeader, zf_defineinCameraHeader);
        DX::GetDevice()->SetTransform(D3DTS_PROJECTION, &m_matProj);
    }
    
    
}
