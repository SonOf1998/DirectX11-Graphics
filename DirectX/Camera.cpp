#include "pch.h"
#include "Camera.h"

Camera::Camera(const XMVECTOR& position) : position(position)
{
	
}

XMVECTOR Camera::GetPosition() const noexcept
{
	return position;
}

XMMATRIX Camera::GetViewProjMatrix() const noexcept
{
	return viewProjMatrix;
}

XMMATRIX Camera::GetViewProjMatrixInv() const noexcept
{
	return viewProjMatrixInv;
}

void Camera::SetWhiteBallPos(const XMVECTOR& v) noexcept
{
	whiteBallPos = v;
}

void Camera::SetTargetBallPos(const XMVECTOR& v) noexcept
{
	targetBallPos = v;
}

XMVECTOR Camera::GetWhiteBallPos() const
{
	return whiteBallPos;
}

XMVECTOR Camera::GetTargetBallPos() const
{
	return targetBallPos;
}
