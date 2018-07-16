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
    : m_pTarget(nullptr)
    , m_pCurrObjPos(D3DXVECTOR3(0,0,0))
    , m_distance(1000.0f)
    , m_basePosY(200.0f)
    , m_basePosX(0.0f)
    , m_up(D3DXVECTOR3(0, 1, 0))
    , m_rotX(0.0f)
    , m_rotY(0.0f)
    , isPerspective(true)
    , isHandle(false)
    , m_viewWidth(10.0f)
    , m_SavedLookAt(D3DXVECTOR3(0, 0, 0))
    , m_fov(D3DX_PI / 4.0f)
{
	m_eye = D3DXVECTOR3(m_basePosX, m_basePosY, -m_distance);
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

void Camera::Update()
{
    Mouse* pMouse = Mouse::Get();
    Keyboard* pKeyboard = Keyboard::Get();

    if (*m_operation != ImGuizmo::NOTSELECTED)
    {
        //��ũ�Ѹ����� m_eye ��ġ�� �ٲٴ� �κ�
        m_distance -= pMouse->GetDeltaPosition().z / 2.0f;
    }
    

    m_moveDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    if (pKeyboard->KeyUp('F'))
    {
        PressFtoMovetoTarget();
    }


    bool isAltKeyPressed = pKeyboard->KeyPress(VK_MENU);
    if (isAltKeyPressed)
    {
        if (pMouse->ButtonPress(Mouse::LBUTTON))
        {
            m_rotX += (pMouse->GetDeltaPosition().y) / 100.f;
            m_rotY += (pMouse->GetDeltaPosition().x) / 100.f;

            //X�� ȸ�� ����
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



            if (isHandle&&pMouse->ButtonPress(Mouse::LBUTTON))
            {


                const float factor = 10.0f;


                //screen Y ��
                if (pMouse->GetDeltaPosition().y > 0.0f)
                {
                    m_moveDir += getUp()*factor;

                }
                else if (pMouse->GetDeltaPosition().y < 0.0f)
                {
                    m_moveDir += getDown()*factor;
                }


                //screen X ��
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





    //�� space
    D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_SavedLookAt, &m_up);
	DX::GetDevice()->SetTransform(D3DTS_VIEW, &m_matView);
    //�������� space
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
