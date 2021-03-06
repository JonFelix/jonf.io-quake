//#define _CRT_SECURE_NO_WARNINGS


#include "quakedef.h"
#include "winquake.h"


static int BufferWidth = 0;
static int BufferHeight = 0;
static int WindowWidth = 0;
static int WindowHeight = 0;
static int BytesPerPixel = 4;
void* BackBuffer = NULL;
HWND MainWindow = { 0 };

typedef enum { MS_WINDOWED, MS_FULLSCREEN } modestate_t;

typedef struct
{
	modestate_t type;
	int32 width;
	int32 height;
	uint32 Hz;
} vmode_t;

vmode_t ModeList[40];
int32 ModeCount = 0;
int32 FirstFullscreenMode = -1;

BITMAPINFO BitMapInfo = { 0 };

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;

	// catch any relevant messages here
	switch (uMsg)
	{
	case WM_ACTIVATE:

		break;

	case WM_CREATE:

		break;

	case WM_DESTROY:
		//sys_Shutdown();
		break;

	case WM_KEYDOWN:
	{
		if (wParam == 'A')
		{
			vid_SetMode(0);
		}
		else if (wParam == 'S')
		{
			vid_SetMode(1);
		}
		else if (wParam == 'D')
		{
			vid_SetMode(2);
		}
		else if (wParam == 'F')
		{
			vid_SetMode(3);
		}
		else if (wParam == '1')
		{
			vid_SetMode(FirstFullscreenMode);
		}
		else if (wParam == '2')
		{
			vid_SetMode(FirstFullscreenMode + 1);
		}
		else if (wParam == 'Q')
		{
			sys_Shutdown();
		}

	} break;

	default:
		Result = DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return Result;
}

void vid_InitWindowedMode()
{
	ModeList[ModeCount].type = MS_WINDOWED;
	ModeList[ModeCount].width = 320;
	ModeList[ModeCount].height = 240;
	ModeList[ModeCount].Hz = 0;
	ModeCount++;

	ModeList[ModeCount].type = MS_WINDOWED;
	ModeList[ModeCount].width = 640;
	ModeList[ModeCount].height = 480;
	ModeList[ModeCount].Hz = 0;
	ModeCount++;

	ModeList[ModeCount].type = MS_WINDOWED;
	ModeList[ModeCount].width = 800;
	ModeList[ModeCount].height = 600;
	ModeList[ModeCount].Hz = 0;
	ModeCount++;

	ModeList[ModeCount].type = MS_WINDOWED;
	ModeList[ModeCount].width = 1024;
	ModeList[ModeCount].height = 768;
	ModeList[ModeCount].Hz = 0;
	ModeCount++;
}

void vid_InitFullscreenMode()
{
	DEVMODE DevMode;
	BOOL Success;
	int ModeNum = 0;
	int OldWidth = 0, OldHeight = 0;

	FirstFullscreenMode = ModeCount;

	do
	{
		Success = EnumDisplaySettings(NULL, ModeNum, &DevMode);

		if (DevMode.dmPelsHeight == OldHeight && DevMode.dmPelsWidth == OldWidth)
		{
			if (DevMode.dmDisplayFrequency > ModeList[ModeCount - 1].Hz)
			{
				ModeList[ModeCount - 1].Hz = DevMode.dmDisplayFrequency;
			}
			ModeNum++;
		}
		else
		{
			ModeList[ModeCount].type = MS_FULLSCREEN;
			ModeList[ModeCount].width = DevMode.dmPelsWidth;
			ModeList[ModeCount].height = DevMode.dmPelsHeight;
			ModeList[ModeCount].Hz = DevMode.dmDisplayFrequency;
			ModeCount++;

			OldHeight = DevMode.dmPelsHeight;
			OldWidth = DevMode.dmPelsWidth;
			ModeNum++;
		}

	} while (Success);
}

void vid_SetWindowedMode(int ModeValue)
{
	DWORD WindowExStyle = 0;
	DWORD WindowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	// create rectangle for window
	RECT r = { 0 };
	r.right = WindowWidth;
	r.bottom = WindowHeight;
	AdjustWindowRectEx(&r, WindowStyle, 0, WindowExStyle);

	// create our window
	MainWindow = CreateWindowEx(
		WindowExStyle, "JonQuake",
		"jQuake", WindowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		r.right - r.left, r.bottom - r.top,
		NULL, NULL,
		globalHInstance, 0);
}

void vid_SetFullscrenMode(int ModeValue)
{
	DWORD WindowExStyle = 0;
	DWORD WindowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	BOOL Fullscreen = TRUE;

	if (Fullscreen)
	{
		DEVMODE dmScreenSettings = { 0 };
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = ModeList[ModeValue].width;
		dmScreenSettings.dmPelsHeight = ModeList[ModeValue].height;
		dmScreenSettings.dmDisplayFrequency = ModeList[ModeValue].Hz;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
		{
			WindowExStyle = WS_EX_APPWINDOW;
			WindowStyle = WS_POPUP;
		}
		else
		{
			Fullscreen = FALSE;
		}
	}

	// create rectangle for window
	RECT r = { 0 };
	r.right = WindowWidth;
	r.bottom = WindowHeight;
	AdjustWindowRectEx(&r, WindowStyle, 0, WindowExStyle);

	// create our window
	MainWindow = CreateWindowEx(
		WindowExStyle, "JonQuake",
		"jQuake", WindowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		r.right - r.left, r.bottom - r.top,
		NULL, NULL,
		globalHInstance, 0);

	SetWindowLong(MainWindow, GWL_STYLE, 0);
}

void vid_SetMode(int ModeValue)
{
	if (BackBuffer)
	{
		vid_Shutdown();
	}

	WindowWidth = ModeList[ModeValue].width;
	WindowHeight = ModeList[ModeValue].height;

	BufferHeight = WindowHeight;
	BufferWidth = WindowWidth;

	if (ModeList[ModeValue].type == MS_WINDOWED)
	{
		vid_SetWindowedMode(ModeValue);
	}
	else
	{
		vid_SetFullscrenMode(ModeValue);
	}

	ShowWindow(MainWindow, SW_SHOWDEFAULT);

	HDC DeviceContext = GetDC(MainWindow);
	PatBlt(DeviceContext, 0, 0, BufferWidth, BufferHeight, BLACKNESS);
	ReleaseDC(MainWindow, DeviceContext);

	// define our bitmap info
	BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);
	BitMapInfo.bmiHeader.biWidth = BufferWidth;
	BitMapInfo.bmiHeader.biHeight = -BufferHeight;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 8 * BytesPerPixel;
	BitMapInfo.bmiHeader.biCompression = BI_RGB;

	BackBuffer = malloc(BufferWidth * BufferHeight * BytesPerPixel);
}

void vid_Init(void)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = globalHInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "JonQuake";

	if (!RegisterClassEx(&wc))
		exit(EXIT_FAILURE);

	vid_InitWindowedMode();
	vid_InitFullscreenMode();

	vid_SetMode(0);
}

void vid_Update(void)
{
	DWORD error = GetLastError();
	HDC dc = GetDC(MainWindow);
	StretchDIBits(dc,
		0, 0, WindowWidth, WindowHeight,
		0, 0, BufferWidth, BufferHeight,
		BackBuffer, (BITMAPINFO*)&BitMapInfo,
		DIB_RGB_COLORS, SRCCOPY);
	ReleaseDC(MainWindow, dc);
}

void vid_Shutdown(void)
{
	ChangeDisplaySettings(NULL, 0);
	DestroyWindow(MainWindow);
	free(BackBuffer);
	BackBuffer = NULL;
}