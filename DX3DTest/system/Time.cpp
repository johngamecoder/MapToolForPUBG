#include "stdAfx.h"
#include "Time.h"

Time* Time::instance = NULL;

Time * Time::Get()
{
	if (instance == NULL)
		instance = new Time();

	return instance;
}

void Time::Delete()
{
	SAFE_DELETE(instance);
}

Time::Time()
{
	prevTime = GetTickCount();
	sumTime = 0;
	frameCount = 0;
}

Time::~Time()
{
}

void Time::Update()
{
	auto currentTime = GetTickCount();
	deltaTime = currentTime - prevTime;
	prevTime = currentTime;

	frameCount++;
	sumTime += deltaTime;
	if (sumTime >= 200)
	{
		fps = frameCount / sumTime * 1000.0f;
		frameCount = 0;
		sumTime = 0;
	}
	else
	{
		//Sleep(10);
	}

	Debug::Get()->AddText("FPS : ");
	Debug::Get()->AddText(fps);
	Debug::Get()->EndLine();
}



