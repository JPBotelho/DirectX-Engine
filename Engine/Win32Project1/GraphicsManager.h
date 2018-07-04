#pragma once
#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>
#include <stdio.h>

using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

class GraphicsManager
{
	public:
		GraphicsManager(float x, float y);
		~GraphicsManager();
		HRESULT InitializeGraphics(HWND hWnd);
		HRESULT InitD3D(HWND hWnd);
		void EndD3D();
		void RenderFrame();
		HRESULT InitShaders(bool clearLog);
		HRESULT CreateConstBuffer();
		HRESULT SetTexture();
		void UpdateConstBuffer();
		float deltaTime;
		float timeElapsed;
};