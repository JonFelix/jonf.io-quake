#pragma once
#include "winquake.h"
#include "quakedef.h"



#include "host.h"

HINSTANCE globalHInstance;


static BOOL IsRunning = TRUE;


void sys_Shutdown(void)
{
	IsRunning = FALSE;

}

void sys_SendKeyEvents(void)
{
	MSG msg;
	while (PeekMessage(&msg, 0, 0, NULL, PM_REMOVE))
	{
		TranslateMessage(&msg);
		 DispatchMessage(&msg);
	}
}


static __int64 GTimeCount = 0;
static double GTimePassed = 0;
static double SecondsPerTick = 0;









float sys_InitFloatTime(void)
{
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);

	SecondsPerTick = 1.0 / (double)Frequency.QuadPart;

	LARGE_INTEGER Counter;
	QueryPerformanceCounter(&Counter);
	GTimeCount = Counter.QuadPart;
	return 0;
}

float sys_FloatTime(void)
{
	LARGE_INTEGER Counter;
	QueryPerformanceCounter(&Counter);

	__int64 interval = Counter.QuadPart - GTimeCount;
	GTimeCount = Counter.QuadPart;
	double SecondsGoneBy = (double)interval * SecondsPerTick;
	GTimePassed += SecondsGoneBy;
	return (float)GTimePassed;
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	globalHInstance = hInstance;

	COM_ParseCmdLine(lpCmdLine);
	
	host_Init();

	float oldtime = sys_InitFloatTime();

	while (IsRunning)
	{
		float newtime = sys_FloatTime();
		host_Frame(newtime - oldtime);
		oldtime = newtime;
	}

	host_Shutdown();
	return EXIT_SUCCESS;
}