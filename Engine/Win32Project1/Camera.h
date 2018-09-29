/*
 * Class to create matrices used for rendering based on position and rotation.
 */

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
	void AddPosition(D3DXVECTOR3);
private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	float nearPlane, farPlane;
	float horFOV, vertFOV;

};

