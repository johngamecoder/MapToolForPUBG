#pragma once

#define MAX_INPUT_KEY 255

class Keyboard
{
private:
	static Keyboard* instance;

	byte keyState[MAX_INPUT_KEY];
	byte keyOldState[MAX_INPUT_KEY];
	byte keyMap[MAX_INPUT_KEY];

	enum
	{
		KEY_INPUT_STATUS_NONE = 0,
		KEY_INPUT_STATUS_DOWN,
		KEY_INPUT_STATUS_UP,
		KEY_INPUT_STATUS_PRESS,
	};

	Keyboard();
	~Keyboard();

public:
	static Keyboard* Get();
	static void Delete();

	void Update();

	bool KeyDown(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_DOWN; }
	bool KeyUp(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_UP; }
	bool KeyPress(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_PRESS; }
};