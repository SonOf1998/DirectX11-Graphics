#include "pch.h"
#include "WhiteBallObject.h"

#include "PerspectiveCamera.h"
#include "Positions.h"


void WhiteBallObject::Animate(float t, float dt)
{
	//	Logger::print(std::to_string(XMVectorGetX(position)) + " , " + std::to_string(XMVectorGetZ(position)));ww

	if (InputClass::IsKeyDown(VK_UP))
	{
		position += XMVectorSet(0, 0, -1, 0) * dt;
	}
	if (InputClass::IsKeyDown(VK_DOWN))
	{
		position -= XMVectorSet(0, 0, -1, 0) * dt;
	}
	if (InputClass::IsKeyDown(VK_RIGHT))
	{
		position += XMVectorSet(1, 0, 0, 0) * dt;
	}
	if (InputClass::IsKeyDown(VK_LEFT))
	{
		position -= XMVectorSet(1, 0, 0, 0) * dt;
	}

	Logger::print(std::to_string(XMVectorGetX(position)) + " " + std::to_string(XMVectorGetZ(position)));

	if (InputClass::IsKeyDown(VK_SPACE))
	{
		XMFLOAT3 velocityF3 = PerspectiveCamera::GetForward();
		velocityF3.y = 0.0;
		velocity = XMVector3Normalize(XMLoadFloat3(&velocityF3));

		//velocity = XMVectorSet(0, 0, -3, 0);
	}

	BallObject::Animate(t, dt);
}