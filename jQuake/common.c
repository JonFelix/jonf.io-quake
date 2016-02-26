#include "quakedef.h"

int32 com_argc = 0;
uint8 *com_argv[MAX_NUM_ARGV + 1];

int32 COM_CheckParm(uint8 *parm)
{
	for (int32 i = 0; i < com_argc; i++)
	{
		if (!Q_strcmp(parm, com_argv[i]))
		{
			return i;
		}
	}

	return 0;
}

void COM_ParseCmdLine(uint8 *lpCmdLine)
{
	com_argv[0] = 0;
	while (*lpCmdLine && (com_argc < MAX_NUM_ARGV + 1))
	{
		while (*lpCmdLine && ((*lpCmdLine <= 32) || (*lpCmdLine > 127)))
		{
			lpCmdLine++;
		}

		if (*lpCmdLine)
		{
			com_argv[com_argc] = lpCmdLine;
			com_argc++;

			while (*lpCmdLine && ((*lpCmdLine > 32) && (*lpCmdLine <= 127)))
			{
				lpCmdLine++;
			}

			if (*lpCmdLine)
			{
				*lpCmdLine = 0;
				lpCmdLine++;
			}
		}
	}
}