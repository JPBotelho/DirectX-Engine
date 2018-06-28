#include "stdafx.h"
#include <windows.h>
#include "Win32Project1.h"
#include "WindowManager.h"
#include "GraphicsManager.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	WindowManager *manager = new WindowManager;
	manager->DisplayWindow(hInstance, nCmdShow);

	return 0;
}
