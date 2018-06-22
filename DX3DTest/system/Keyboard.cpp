#include "stdafx.h"
#include "Keyboard.h"

Keyboard* Keyboard::instance = NULL;

Keyboard * Keyboard::Get()
{
	if (instance == NULL)
		instance = new Keyboard();

	return instance;
}

void Keyboard::Delete()
{
	SAFE_DELETE(instance);
}

Keyboard::Keyboard()
{
	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyOldState, sizeof(keyOldState));
	ZeroMemory(keyMap, sizeof(keyMap));
}

Keyboard::~Keyboard()
{

}

void Keyboard::Update()
{
	memcpy(keyOldState, keyState, sizeof(keyOldState));
	
	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyMap, sizeof(keyMap));
	
	GetKeyboardState(keyState);

	for (DWORD i = 0; i < MAX_INPUT_KEY; i++)
	{
		byte key = keyState[i] & 0x80;
		keyState[i] = key ? 1 : 0;

		int oldState = keyOldState[i];
		int state = keyState[i];

		if (oldState == 0 && state == 1)
			keyMap[i] = KEY_INPUT_STATUS_DOWN; //이전 0, 현재 1 - KeyDown
		else if (oldState == 1 && state == 0)
			keyMap[i] = KEY_INPUT_STATUS_UP; //이전 1, 현재 0 - KeyUp
		else if (oldState == 1 && state == 1)
			keyMap[i] = KEY_INPUT_STATUS_PRESS; //이전 1, 현재 1 - KeyPress
		else 
			keyMap[i] = KEY_INPUT_STATUS_NONE;
	}
}


