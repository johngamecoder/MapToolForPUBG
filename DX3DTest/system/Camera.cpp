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
    m_basePosX = 0.0f;
	m_eye = D3DXVECTOR3(m_basePosX, m_basePosY, -m_distance);
    m_lookAt = D3DXVECTOR3(0, 0, 0);
    m_SavedLookAt = m_lookAt;
    //m_lookAt = D3DXVECTOR3(m_eye.x, m_eye.y - m_basePosY, m_eye.y + m_distance);
	//m_lookAt = D3DXVECTOR3(m_eye.x, m_eye.y, m_eye.z+1);


    m_up = D3DXVECTOR3(0, 1, 0);
	m_rotX = 0.0f;
	m_rotY = 0.0f;
    m_fov = D3DX_PI / 4.0f;

    isPerspective = true;
    isHandle = false;
    m_viewWidth = 10.f;
}


Camera::~Camera()
{
}

void Camera::Init()
{
	GetClientRect(DX::GetInfo().hWnd, &m_rect);

	D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_SavedLookAt, &m_up);
	DX::GetDevice()->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fov,
        m_rect.right / (float)m_rect.bottom, zn_defineinCameraHeader, zf_defineinCameraHeader);
	DX::GetDevice()->SetTransform(D3DTS_PROJECTION, &m_matProj);
}
float X = 0;
float Y = 0;
void Camera::Update()
{
    bool isAltKeyPressed = Keyboard::Get()->KeyPress(VK_MENU);
    if (isAltKeyPressed)
    {
        if ( Mouse::Get()->ButtonPress(Mouse::LBUTTON))
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
    else
    {
        if (isHandle &&Mouse::Get()->ButtonPress(Mouse::LBUTTON))
        {
            Y += (Mouse::Get()->GetDeltaPosition().y) / 100.f;
            X -= (Mouse::Get()->GetDeltaPosition().x) / 100.f;
        }
    }


    m_distance -= Mouse::Get()->GetDeltaPosition().z / 120.0f;
    m_distance = max(2, m_distance);
    m_distance = min(100, m_distance);



    m_lookAt = D3DXVECTOR3(X, Y, 0);
    m_eye = D3DXVECTOR3(0, m_basePosY, -m_distance);


    D3DXMATRIXA16 matRot;
    D3DXMatrixRotationYawPitchRoll(&matRot, m_rotY, m_rotX, 0);



    if(!isAltKeyPressed)
        D3DXVec3TransformCoord(&m_lookAt, &m_lookAt, &matRot);
    D3DXVec3TransformCoord(&m_eye, &m_eye, &matRot);







    //m_lookAt = D3DXVECTOR3(X, Y, 0);
    m_eye = m_lookAt + m_eye;
    m_SavedLookAt = m_lookAt;
	
    
    
    
    
    
    
    D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_SavedLookAt, &m_up);
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

//m_distance -= Mouse::Get()->GetDeltaPosition().z / 120.0f;
//m_distance = max(2, m_distance);
//m_distance = min(100, m_distance);
//
//bool isAltKey = false;
//isAltKey = Keyboard::Get()->KeyPress(VK_MENU);
//
//if (isAltKey)
//{
//    if (Mouse::Get()->ButtonPress(Mouse::LBUTTON))
//    {
//        m_rotX += (Mouse::Get()->GetDeltaPosition().y) / 100.f;
//        m_rotY += (Mouse::Get()->GetDeltaPosition().x) / 100.f;
//
//        //X축 회전 제한
//        if (m_rotX <= -D3DX_PI * 0.4f)
//        {
//            m_rotX = -D3DX_PI * 0.4f;
//        }
//        if (m_rotX >= D3DX_PI * 0.35f)
//        {
//            m_rotX = D3DX_PI * 0.35f;
//        }
//    }
//    m_eye = D3DXVECTOR3(m_basePosX, m_basePosY, -m_distance);
//}
//else
//{
//    if (isHandle && Mouse::Get()->ButtonPress(Mouse::LBUTTON))
//    {
//        m_basePosY += (Mouse::Get()->GetDeltaPosition().y) / 100.f;
//        m_basePosX -= (Mouse::Get()->GetDeltaPosition().x) / 100.f;
//        m_eye = D3DXVECTOR3(m_basePosX, m_basePosY, -m_distance);
//        m_lookAt = D3DXVECTOR3(m_eye.x, m_eye.y - m_basePosY, m_eye.y + m_distance);
//        m_SavedLookAt = m_lookAt;
//    }
//    else
//    {
//        m_eye = D3DXVECTOR3(m_basePosX, m_basePosY, -m_distance);
//    }
//}
//
//
//
//
//
//
//
//D3DXMATRIXA16 matR, matT, mat;
//D3DXMatrixRotationYawPitchRoll(&matR, m_rotY, m_rotX, 0);
////D3DXMatrixTranslation(&matT, m_tranX, m_tranY, 0);
//D3DXMatrixIdentity(&matT);
//mat = matR * matT;
//
//D3DXVec3TransformCoord(&m_eye, &m_eye, &mat);
//if (!isAltKey)
//D3DXVec3TransformCoord(&m_lookAt, &m_lookAt, &mat);
//else
//m_eye = m_SavedLookAt + m_eye;
//
//
//if (m_pTarget)
//{
//    m_lookAt = *m_pTarget;
//    m_eye = *m_pTarget + m_eye;
//}