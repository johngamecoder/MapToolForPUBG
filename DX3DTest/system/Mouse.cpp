#include "stdafx.h"
#include "Mouse.h"

Mouse* Mouse::instance = NULL;

Mouse* Mouse::Get()
{
	if ( instance == NULL )
		instance = new Mouse();

	return instance;
}

void Mouse::Delete()
{
	SAFE_DELETE(instance);
}

Mouse::Mouse()
{
	
}

Mouse::~Mouse()
{
}

void Mouse::Init(HWND handle)
{
	hWnd = handle;

	position = D3DXVECTOR3(0, 0, 0);
	currPosition = D3DXVECTOR3(0, 0, 0);
	oldPosition = D3DXVECTOR3(0, 0, 0);
	deltaPosition = D3DXVECTOR3(0, 0, 0);

	ZeroMemory(buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonOldStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonMap, sizeof(byte) * MAX_INPUT_MOUSE);

	ZeroMemory(startDblClk, sizeof(DWORD) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonCount, sizeof(int) * MAX_INPUT_MOUSE);

	timeDblClk = GetDoubleClickTime();
	startDblClk[0] = GetTickCount();

	for (int i = 1; i < MAX_INPUT_MOUSE; i++)
		startDblClk[i] = startDblClk[0];

	DWORD line = 0;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &line, 0);
}

void Mouse::Update()
{
       
	memcpy(buttonOldStatus, buttonStatus, sizeof(buttonOldStatus));
	
	ZeroMemory(buttonStatus, sizeof(buttonStatus));
	ZeroMemory(buttonMap, sizeof(buttonMap));

	buttonStatus[INPUT_TYPE::LBUTTON] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[INPUT_TYPE::RBUTTON] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[INPUT_TYPE::MBUTTON] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	
	for ( DWORD i = 0; i < MAX_INPUT_MOUSE; i++ )
	{
		int oldStatus = buttonOldStatus[i];
		int currentStatus = buttonStatus[i];

		if ( oldStatus == 0 && currentStatus == 1 ) 
			buttonMap[i] = BUTTON_INPUT_STATUS_DOWN;
		else if ( oldStatus == 1 && currentStatus == 0 ) 
			buttonMap[i] = BUTTON_INPUT_STATUS_UP;
		else if ( oldStatus == 1 && currentStatus == 1 )
			buttonMap[i] = BUTTON_INPUT_STATUS_PRESS;
		else 
			buttonMap[i] = BUTTON_INPUT_STATUS_NONE;
	}

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);

	oldPosition.x = currPosition.x;
	oldPosition.y = currPosition.y;

	currPosition.x = float(point.x);
	currPosition.y = float(point.y);

	deltaPosition = currPosition - oldPosition;
	oldPosition.z = currPosition.z;
	
	DWORD buttonDownTime = GetTickCount();
	for ( DWORD i = 0; i < MAX_INPUT_MOUSE; i++ )
	{
		if ( buttonMap[i] == BUTTON_INPUT_STATUS_DOWN )
		{
			if ( buttonCount[i] == 1 )
			{
				if ( (buttonDownTime - startDblClk[i]) >= timeDblClk )
					buttonCount[i] = 0;
			}
			buttonCount[i]++;

			if ( buttonCount[i] == 1 )
				startDblClk[i] = buttonDownTime;
		}

		if ( buttonMap[i] == BUTTON_INPUT_STATUS_UP )
		{
			if ( buttonCount[i] == 1 )
			{
				if ( (buttonDownTime - startDblClk[i]) >= timeDblClk )
					buttonCount[i] = 0;
			}
			else if ( buttonCount[i] == 2 )
			{
				if ( (buttonDownTime - startDblClk[i]) <= timeDblClk )
					buttonMap[i] = BUTTON_INPUT_STATUS_DBLCLK;

				buttonCount[i] = 0;
			}
		}
	}
}

LRESULT Mouse::InputProc(UINT message, WPARAM wParam, LPARAM lParam)
{

    //if (message == WM_LBUTTONDOWN || message == WM_MOUSEMOVE)
    //{
    //    position.x = (float)LOWORD(lParam);
    //    position.y = (float)HIWORD(lParam);
    //}

	

	if ( message == WM_MOUSEWHEEL )
	{
		oldPosition.z = currPosition.z;
		currPosition.z += (float)GET_WHEEL_DELTA_WPARAM(wParam);
	}
	
	return TRUE;
}
