#pragma once
#include <d3d11.h>
#include <d3dx10math.h>

class Camera
{
public:
	Camera();
	~Camera();
	void GetViewMatrix(D3DXMATRIX*);
	void GetProjectionMatrix(D3DXMATRIX*);
	void GetWorldMatrix(D3DXMATRIX*);
	void GetOrthoMatrix(D3DXMATRIX*);
private:
	D3DVECTOR position;
	D3DVECTOR rotation;
	float nearPlane, farPlane;
	float horFOV, vertFOV;

};

