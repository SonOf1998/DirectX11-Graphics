#include "pch.h"
#include "PerspectiveCamera.h"

#include "CBufferDataType.h"
#include "InputClass.h"
#include "SimpleDXMath.h"


PerspectiveCamera::PerspectiveCamera(const XMVECTOR& position, const XMVECTOR& lookAt, float aspectRatio, float fovy, float zNear, float zFar) : Camera(position), aspectRatio(aspectRatio), fovy(fovy), zNear(zNear), zFar(zFar)
{
	SetLookAt(lookAt);	// sets yaw and pitch
	roll = 0.0f;
}

PerspectiveCamera::PerspectiveCamera(const XMVECTOR& position, float aspectRatio, float yaw, float pitch, float roll, float fovy, float zNear, float zFar) : Camera(position), aspectRatio(aspectRatio), yaw(yaw), pitch(pitch), roll(roll), fovy(fovy), zNear(zNear), zFar(zFar)
{

}

float PerspectiveCamera::GetYaw() const noexcept
{
	return yaw;
}

void PerspectiveCamera::SetYaw(float rad) noexcept
{
	yaw = rad;
}

void PerspectiveCamera::SetYawDeg(float deg) noexcept
{
	SetYaw(deg * PI_180);
}

float PerspectiveCamera::GetPitch() const noexcept
{
	return pitch;
}

void PerspectiveCamera::SetPitch(float rad) noexcept
{
	pitch = rad;
}

void PerspectiveCamera::SetPitchDeg(float deg) noexcept
{
	SetPitch(deg * PI_180);
}

float PerspectiveCamera::GetRoll() const noexcept
{
	return roll;
}

void PerspectiveCamera::SetRoll(float rad) noexcept
{
	roll = rad;
}

void PerspectiveCamera::SetRollDeg(float deg) noexcept
{
	SetRoll(deg * PI_180);
}

void PerspectiveCamera::SetPosition(const XMVECTOR& newPos) noexcept
{
	position = newPos;
}

void PerspectiveCamera::SetLookAt(const XMVECTOR& lookAt)
{
	assert(lookAt != position);

	XMVECTOR direction = lookAt - position;
	direction = XMVector3Normalize(direction);
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, direction);

	pitch = asinf(dir.y);
	yaw = atan2f(-dir.x, -dir.z);
}

void PerspectiveCamera::SetAspectRatio(float newAspectRatio) noexcept
{
	aspectRatio = newAspectRatio;
}

XMFLOAT3 PerspectiveCamera::GetPositionF() const noexcept
{
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, position);

	return pos;
}

XMFLOAT3 PerspectiveCamera::GetForward() const noexcept
{
	return XMFLOAT3(-sinf(yaw)*cosf(pitch), sinf(pitch), -cosf(yaw)*cos(pitch));
}

XMFLOAT3 PerspectiveCamera::GetUp() const noexcept
{
	return XMFLOAT3(0, 1, 0);
}


XMVECTOR PerspectiveCamera::GetPosition() const noexcept
{
	return position;
}


void PerspectiveCamera::Animate(float t, float dt)
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

	XMMATRIX rotation = XMMatrixTranspose(XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	XMMATRIX invTranslate = XMMatrixTranslationFromVector(-position);

	XMMATRIX view = invTranslate * rotation;
	XMMATRIX projection = XMMatrixPerspectiveFovRH(fovy, aspectRatio, zNear, zFar);

	viewProjMatrix = XMMatrixMultiply(view, projection);
	viewProjMatrixInv = XMMatrixInverse(nullptr, viewProjMatrix);
}