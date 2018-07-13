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
    : m_pTarget(NULL)
    , m_distance(1000.0f)
    , m_basePosY(200.0f)
    , m_basePosX(0.0f)
    , m_up(D3DXVECTOR3(0, 1, 0))
    , m_rotX(0.0f)
    , m_rotY(0.0f)
    , isPerspective(true)
    , isHandle(false)
    , m_viewWidth(10.0f)
{
	m_eye = D3DXVECTOR3(m_basePosX, m_basePosY, -m_distance);
    m_SavedLookAt = D3DXVECTOR3(0, 0, 0);
    m_fov = D3DX_PI / 4.0f;

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
    Mouse* pMouse = Mouse::Get();
    //스크롤링으로 m_eye 위치를 바꾸는 부분
    m_distance -= pMouse->GetDeltaPosition().z / 2.0f;


    m_moveDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);



    bool isAltKeyPressed = Keyboard::Get()->KeyPress(VK_MENU);
    if (isAltKeyPressed)
    {
        if (pMouse->ButtonPress(Mouse::LBUTTON))
        {
            m_rotX += (pMouse->GetDeltaPosition().y) / 100.f;
            m_rotY += (pMouse->GetDeltaPosition().x) / 100.f;

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
        if (isHandle &&pMouse->ButtonPress(Mouse::LBUTTON))
        {
            //Y += pMouse->GetDeltaPosition().y;
            //X -= pMouse->GetDeltaPosition().x;

            //ImGui::Text("%f", pMouse->GetDeltaPosition().y);
            //ImGui::Text("%f", pMouse->GetDeltaPosition().x);


            const float factor = 10.0f;


            //screen Y 축
            if (pMouse->GetDeltaPosition().y > 0.0f)
            {
                m_moveDir += getUp()*factor;

            }
            else if (pMouse->GetDeltaPosition().y < 0.0f)
            {
                m_moveDir += getDown()*factor;
            }


            //screen X 축
            if (pMouse->GetDeltaPosition().x > 0.0f)
            {
                m_moveDir += getLeft()*factor;
            }
            else if (pMouse->GetDeltaPosition().x < 0.0f)
            {
                m_moveDir += getRight()*factor;
            }


        }
    }
    D3DXMatrixRotationYawPitchRoll(&m_matRot, m_rotY, m_rotX, 0);



    m_eye = D3DXVECTOR3(0, m_basePosY, -m_distance);
    D3DXVec3TransformCoord(&m_eye, &m_eye, &m_matRot);

    m_SavedLookAt += m_moveDir;

    m_eye = m_SavedLookAt + m_eye;








    //right vector와 left vector
    // up vector와 down vector을 구해서
    // m_lookat의 position을 update 해 준다. 










    //D3DXVECTOR3 lookAt = D3DXVECTOR3(X, Y, 0);
    //m_eye = D3DXVECTOR3(0, m_basePosY, -m_distance);

    //if(!isAltKeyPressed)
    //    D3DXVec3TransformCoord(&lookAt, &lookAt, &m_matRot);
    //D3DXVec3TransformCoord(&m_eye, &m_eye, &m_matRot);

    //m_eye = lookAt + m_eye;
    //
    //
    //
    //
    //m_SavedLookAt = lookAt;
	

    //뷰 space
    D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_SavedLookAt, &m_up);
	DX::GetDevice()->SetTransform(D3DTS_VIEW, &m_matView);
    //프로젝션 space
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
