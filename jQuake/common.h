#pragma once

#define MAX_NUM_ARGV 50

typedef enum { true, false } qboolean;

extern int32 com_argc;
extern uint8 *com_argv[MAX_NUM_ARGV + 1];

int32 COM_CheckParm(char *parm);
void COM_ParseCmdLine(char *lpCmdLine);