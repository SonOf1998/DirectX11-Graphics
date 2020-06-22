#include "pch.h"
#include "WhiteBallObject.h"

#include "Positions.h"

void WhiteBallObject::Animate(float t, float dt)
{
//	Logger::print(std::to_string(XMVectorGetX(position)) + " , " + std::to_string(XMVectorGetZ(position)));ww

	if (InputClass::IsKeyDown(VK_UP) && XMVectorGetZ(position) > -CUSHION_Z_BORDER)
	{
		position += XMVectorSet(0, 0, -1, 0) * dt / 2;
	}
	if (InputClass::IsKeyDown(VK_DOWN) && XMVectorGetZ(position) < CUSHION_Z_BORDER)
	{
		position -= XMVectorSet(0, 0, -1, 0) * dt / 2;
	}
	if (InputClass::IsKeyDown(VK_RIGHT) && XMVectorGetX(position) < CUSHION_X_BORDER)
	{
		position += XMVectorSet(1, 0, 0, 0) * dt / 10;
	}
	if (InputClass::IsKeyDown(VK_LEFT) && XMVectorGetX(position) > -CUSHION_X_BORDER)
	{
		position -= XMVectorSet(1, 0, 0, 0) * dt / 10;
	}


	modelMatrix = XMMatrixScalingFromVector(scale) * rotation * XMMatrixTranslationFromVector(position);
}