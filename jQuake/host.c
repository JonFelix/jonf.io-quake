#include "quakedef.h"

double realtime = 0;
double oldrealtime = 0;
double host_FrameTime = 0;

qboolean host_FilterTime(float time)
{
	realtime += time;

	if (realtime - oldrealtime < 1.0 / 72.0)
	{
		return false;
	}
	host_FrameTime = realtime - oldrealtime;
	oldrealtime = realtime;
	return true;
}

void host_Init(void)
{
	vid_Init();
}

void host_Frame(float timestep)
{
	if (!host_FilterTime(timestep))
	{
		return;
	}

	sys_SendKeyEvents();


	vid_Update();

	// UPDATE GAME

	//RENDER GAME
}

void host_Shutdown(void)
{
	vid_Shutdown();
}