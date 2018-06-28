#pragma once
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
		HRESULT CreateVertBuffer();
		HRESULT CreateConstBuffer();
		void UpdateConstBuffer();
		float deltaTime;
		float timeElapsed;
};