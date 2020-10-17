#include "pch.h"
#include "PerspectiveCamera.h"

// from DirectX
#include "CBufferDataType.h"
#include "InputClass.h"
#include "SimpleDXMath.h"
#include "WhiteBallObject.h"

// from SnookerLogic
#include "RoundManager.h"


PerspectiveCamera::PerspectiveCamera(const XMVECTOR& position, const XMVECTOR& lookAt, float aspectRatio, float fovy, float zNear, float zFar) : Camera(position), aspectRatio(aspectRatio), fovy(fovy), zNear(zNear), zFar(zFar)
{
	defaultPos = position;
	SetLookAt(lookAt);	// sets yaw and pitch
	roll = 0.0f;
}

PerspectiveCamera::PerspectiveCamera(const XMVECTOR& position, float aspectRatio, float yaw, float pitch, float roll, float fovy, float zNear, float zFar) : Camera(position), aspectRatio(aspectRatio), fovy(fovy), zNear(zNear), zFar(zFar)
{
	defaultPos = position;
	this->yaw = yaw;
	this->pitch = pitch;
	this->roll = roll;
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

XMFLOAT3 PerspectiveCamera::GetForward() noexcept
{
	return XMFLOAT3(-sinf(yaw)*cosf(pitch), sinf(pitch), -cosf(yaw)*cos(pitch));
}

XMFLOAT3 PerspectiveCamera::GetUp() const noexcept
{
	return XMFLOAT3(0, 1, 0);
}

void PerspectiveCamera::GoAimMode()
{
	XMFLOAT3 wbp;
	XMFLOAT3 tbp;

	XMStoreFloat3(&wbp, whiteBallPos);
	XMStoreFloat3(&tbp, targetBallPos);
	

	// SetPosition first as SetLookAt relies on parameter "position"
	SetPosition(XMVectorSet((wbp.x - tbp.x) + wbp.x, wbp.y, (wbp.z - tbp.z) + wbp.z, 0.0f));
	SetLookAt(whiteBallPos);
	defaultPos = position;
	gyroAngle = 0;
	extraElevation = 0;

	//log
	// Logger::print(wbp);
	// Logger::print(tbp);
}

void PerspectiveCamera::MagnifyAimMode(short level)
{
	static constexpr float magnUnitStep = 0.02f;
	static constexpr float magnMin = 1.25f;
	static constexpr float magnMax = 0.5f;

	float magnificationLevel = level * magnUnitStep;

	if (aimModeMagnification - magnificationLevel > magnMin)
	{
		aimModeMagnification = magnMin;
		return;
	}
	if (aimModeMagnification - magnificationLevel < magnMax)
	{
		aimModeMagnification = magnMax;
		return;
	}

	aimModeMagnification -= magnificationLevel;
}

float PerspectiveCamera::GetGyroAngle() const noexcept
{
	return gyroAngle;
}


XMVECTOR PerspectiveCamera::GetPosition() const noexcept
{
	return position;
}


void PerspectiveCamera::Animate(float t, float dt)
{
	RoundManager& rm = RoundManager::GetInstance();

	if ((WhiteBallObject::isInAimMode || WhiteBallObject::isInFineAimMode) && !rm.IsRoundGoing())
	{
		POINT cursorMove = InputClass::GetCursorMove();
		if (InputClass::LeftMBDown())
		{
			float gyroStep = 0;
			float elevationStep = 0.008f;

			if (WhiteBallObject::isInAimMode)
				gyroStep = 0.002f;
			else if (WhiteBallObject::isInFineAimMode)
				gyroStep = 0.0005f;

			gyroAngle += cursorMove.x * gyroStep;

			if (std::fabsf(cursorMove.y) >= std::fabs(3 * cursorMove.x))
				extraElevation += cursorMove.y * elevationStep;

			extraElevation = std::clamp(extraElevation, -0.33f, 1.1f);
		}
		

		XMVECTOR dir = defaultPos - whiteBallPos;
		float xOld = XMVectorGetX(dir);
		float zOld = XMVectorGetZ(dir);
		float y = XMVectorGetY(dir);

		float xNew = xOld * cosf(gyroAngle) - zOld * sinf(gyroAngle);
		float zNew = xOld * sinf(gyroAngle) + zOld * cosf(gyroAngle);

		// camera offset from cue ball
		// can be adjusted by rolling the mouse's middle button
		float len = Length(whiteBallPos - targetBallPos);
		float distance_factor = aimModeMagnification * 0.4f / (len / 6.0f);
		float elevation = aimModeMagnification * std::max(0.9f * (len / 6.0f), 0.5f) + aimModeMagnification * extraElevation;

		XMVECTOR newDir = XMVectorSet(distance_factor * xNew, y + elevation, distance_factor * zNew, 0);
		position = whiteBallPos + newDir;
		SetLookAt(whiteBallPos);
		
	}
	// walk mode
	else
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

	

	XMMATRIX rotation = XMMatrixTranspose(XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	XMMATRIX invTranslate = XMMatrixTranslationFromVector(-position);

	XMMATRIX view = invTranslate * rotation;
	XMMATRIX projection = XMMatrixPerspectiveFovRH(fovy, aspectRatio, zNear, zFar);

	viewProjMatrix = XMMatrixMultiply(view, projection);
	viewProjMatrixInv = XMMatrixInverse(nullptr, viewProjMatrix);
}