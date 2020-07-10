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
