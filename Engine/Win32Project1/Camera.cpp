#include "stdafx.h"
#include "Camera.h"
#include <d3d11.h>
#include <d3dx10math.h>


Camera::Camera()
{
	position.x = 1.0;
	position.y = 0.0;	
	position.z = -1.0;

	rotation.x = 0.0;
	rotation.y = 0.0;
	rotation.z = 0.0;
}


Camera::~Camera()
{
}

void Camera::AddPosition(D3DXVECTOR3 add)
{
	position += add;
}

void Camera::GetViewMatrix(D3DXMATRIX* viewMatrix)
{
	D3DXVECTOR3 up, pos, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	pos.x = position.x;
	pos.y = position.y;
	pos.z = position.z;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = rotation.x * 0.0174532925f;
	yaw = rotation.y * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = pos + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(viewMatrix, &pos, &lookAt, &up);

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

