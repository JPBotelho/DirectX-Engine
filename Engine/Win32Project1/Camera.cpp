#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	position.x = 0.0;
	position.y = 0.0;	
	position.z = 0.0;
	rotation.x = 0.0;
	rotation.y = 0.0;
	rotation.z = 0.0;
}


Camera::~Camera()
{
}

D3DXMATRIX Camera::GetViewMatrix()
{
	D3DXMATRIX worldMatrix;
	D3DXMatrixIdentity(&worldMatrix);
	return worldMatrix;
}

void Camera::GetProjectionMatrix(D3DXMATRIX* projMatrix)
{
	float fieldOfView = 45 * 0.0174532925f;// (float)D3DX_PI / 4.0f;
	float screenAspect = (float)16.0 / (float)9.0;

	D3DXMatrixPerspectiveFovLH(projMatrix, fieldOfView, screenAspect, .1, 1000);
}

void Camera::GetOrthoMatrix(D3DXMATRIX* orthoMatrix)
{
	D3DXMatrixOrthoLH(orthoMatrix, (float)1920, (float)1080, .1, 1000);
}

void Camera::GetWorldMatrix(D3DXMATRIX* worldMatrix)
{
	D3DXMatrixIdentity(worldMatrix);
}

