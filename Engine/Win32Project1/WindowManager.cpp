#include "stdafx.h"
#include "WindowManager.h"
#include <windows.h>
#include "GraphicsManager.h"
#include <ctime>
#include <chrono>
#include <ratio>





#define _UNICODE

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam,	LPARAM lParam);

const LPCTSTR name = L"WindowClass";
const LPCTSTR title = L"Peanut Engine";
RECT bounds = { 0, 0, 1920, 1080 };
float width = 1920;
float height = 1080;

double oldTime;
double newTime;

double GetTimeMS();

GraphicsManager *graphicsManager = new GraphicsManager(width, height);



WindowManager::WindowManager()
{
}


WindowManager::~WindowManager()
{
}

void WindowManager::DisplayWindow(HINSTANCE hInstance, int nCmdShow)
{
	auto start_time = std::chrono::high_resolution_clock::now();

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = name;

	AdjustWindowRect(&bounds, WS_OVERLAPPEDWINDOW, 0);

	RegisterClassEx(&wc);
	HWND hWnd = CreateWindowEx(NULL,
									name,
									title,
									WS_OVERLAPPEDWINDOW,
									0,
									0,
									width,
									height,
									NULL,    // parent window
									NULL,    // menu
									hInstance,    // handle
									NULL);    // multiple windows

	/*graphicsManager->InitD3D(hWnd);
	graphicsManager->InitShaders(0);
	graphicsManager->CreateVertBuffer();
	graphicsManager->CreateConstBuffer();*/

	HRESULT successful = graphicsManager->InitializeGraphics(hWnd);
	if (FAILED(successful))
	{
		return;
	}

	ShowWindow(hWnd, nCmdShow);

	newTime = GetTimeMS();

	MSG msg;

	while (1)
	{
		oldTime = newTime;
		newTime = GetTimeMS();
		std::chrono::duration<float, std::milli> time_span = std::chrono::high_resolution_clock::now() - start_time;
		(*graphicsManager).timeElapsed = time_span.count();
		(*graphicsManager).deltaTime = (newTime - oldTime);
		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			graphicsManager->RenderFrame();
		}

	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
		case WM_DESTROY:
		{
			// close the application entirely
			graphicsManager->EndD3D();
			PostQuitMessage(0);
			return 0;
		} 
		case WM_KEYDOWN:
		{
			if (wParam == VK_F5)
			{
				graphicsManager->InitShaders(1);
			}
			return 0;
		}
		break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

double GetTimeMS()
{
	return (double)std::time(0);
}
