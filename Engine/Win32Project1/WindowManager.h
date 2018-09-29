/*
 * Creates a Win32 window and calls D3D initialization functions.
 * Handles inputs and draw calls.
 */

#pragma once
class WindowManager
{
	public:
		WindowManager();
		~WindowManager();
		void DisplayWindow(HINSTANCE inst, int nCmdShow);
};

