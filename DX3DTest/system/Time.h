#pragma once

class Time
{
private:
	static Time*	instance;

	DWORD	prevTime;
	DWORD	deltaTime;
	float	sumTime;
	int		frameCount;
	int		fps;

	Time();
	~Time();

public:
	static Time* Get();
	static void Delete();

	void	Update();
	float	GetDeltaTime() { return deltaTime / 1000.f; }
	int		GetFPS() { return fps; }
};

