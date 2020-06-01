#include "pch.h"
#include "Camera.h"


#include "CBufferDataType.h"
#include "InputClass.h"
#include "SimpleDXMath.h"


Camera::Camera(const XMVECTOR& position, const XMVECTOR& lookAt, float aspectRatio, float fovy, float zNear, float zFar) : position(position), aspectRatio(aspectRatio), fovy(fovy), zNear(zNear), zFar(zFar)
{
	SetLookAt(lookAt);	// sets yaw and pitch
	roll = 0.0f;
}

Camera::Camera(const XMVECTOR& position, float aspectRatio, float yaw, float pitch, float roll, float fovy, float zNear, float zFar) : position(position), aspectRatio(aspectRatio), yaw(yaw), pitch(pitch), roll(roll), fovy(fovy), zNear(zNear), zFar(zFar)
{

}

float Camera::GetYaw() const noexcept
{
	return yaw;
}

void Camera::SetYaw(float rad) noexcept
{
	yaw = rad;
}

void Camera::SetYawDeg(float deg) noexcept
{
	SetYaw(deg * PI_180);
}

float Camera::GetPitch() const noexcept
{
	return pitch;
}

void Camera::SetPitch(float rad) noexcept
{
	pitch = rad;
}

void Camera::SetPitchDeg(float deg) noexcept
{
	SetPitch(deg * PI_180);
}

float Camera::GetRoll() const noexcept
{
	return roll;
}

void Camera::SetRoll(float rad) noexcept
{
	roll = rad;
}

void Camera::SetRollDeg(float deg) noexcept
{
	SetRoll(deg * PI_180);
}

void Camera::SetPosition(const XMVECTOR& newPos) noexcept
{
	position = newPos;
}

void Camera::SetLookAt(const XMVECTOR& lookAt)
{
	assert(lookAt != position);

	XMVECTOR direction = lookAt - position;
	direction = XMVector3Normalize(direction);
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, direction);

	pitch = asinf(dir.y);
	yaw = atan2f(-dir.x, -dir.z);
}

void Camera::SetAspectRatio(float newAspectRatio) noexcept
{
	aspectRatio = newAspectRatio;
}


XMVECTOR Camera::GetPosition() const noexcept
{
	return position;
}

XMMATRIX Camera::GetViewProjMatrix() const
{
	return viewProjMatrix;
}

CameraCBuffer Camera::GetCBuffer()
{
	return CameraCBuffer(position);
}


void Camera::Update()
{
	XMMATRIX rotation = XMMatrixTranspose(XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	XMMATRIX invTranslate = XMMatrixTranslationFromVector(-position);

	XMMATRIX view = XMMatrixMultiply(invTranslate, rotation);
	XMMATRIX projection = XMMatrixPerspectiveFovRH(fovy, aspectRatio, zNear, zFar);
	
	viewProjMatrix = XMMatrixMultiply(view, projection);
}

void Camera::Animate(float t, float dt)
{
	XMVECTOR velocity = XMVectorSet(0, 0, -1, 0);
	XMVECTOR sideVelocity = XMVectorSet(1, 0, 0, 0);

	if (InputClass::IsKeyDown('W'))
	{
		position += ahead * dt;
	}
	if (InputClass::IsKeyDown('S'))
	{
		position -= ahead * dt;
	}
	if (InputClass::IsKeyDown('A'))
	{
		position -= right * dt;
	}
	if (InputClass::IsKeyDown('D'))
	{
		position += right * dt;
	}

	POINT cursorMove = InputClass::GetCursorMove();
	if (InputClass::RightMBDown())
	{
		yaw -= cursorMove.x * 0.002f;
		pitch -= cursorMove.y * 0.002f;
		if (pitch > 3.14f / 2.0f) 
		{
			pitch = 3.14f / 2.0f;
		}
		if (pitch < -3.14f / 2.0f) 
		{
			pitch = -3.14f / 2.0f;
		}

		XMMATRIX rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
		ahead = XMVectorSet(0, 0, -1, 0) * rotation;
		right = XMVectorSet(1, 0, 0, 0) * rotation;
		up = XMVectorSet(0, 1, 0, 0) * rotation;
	}

	
}
