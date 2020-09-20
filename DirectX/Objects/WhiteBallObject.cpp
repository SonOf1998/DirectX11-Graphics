#include "pch.h"
#include "WhiteBallObject.h"

// from DirectX
#include "PerspectiveCamera.h"
#include "Positions.h"

// from SnookerLogic
#include "RoundManager.h"


void WhiteBallObject::SwitchModes()
{
	auto disableAll = [&]() {
		isInAimMode = false;
		isInFineAimMode = false;
		isInSpinMode = false;
		isInShootMode = false;
	};

	switch (lastMode)
	{
	case WHITE_BALL_MODE::AIM:if (isInFineAimMode) {
			disableAll();
			isInFineAimMode = true;
			lastMode = WHITE_BALL_MODE::FINE_AIM;
		}
		else if (isInSpinMode) {
			disableAll();
			isInSpinMode = true;
			lastMode = WHITE_BALL_MODE::SPIN;
		}
		else if (isInShootMode) {
			disableAll();
			isInShootMode = true;
			lastMode = WHITE_BALL_MODE::SHOOT;
		}
		else
		{
			isInAimMode = true;
		}
		break;
	case WHITE_BALL_MODE::FINE_AIM:
		if (isInAimMode) {
			disableAll();
			isInAimMode = true;
			lastMode = WHITE_BALL_MODE::AIM;
		}
		else if (isInSpinMode) {
			disableAll();
			isInSpinMode = true;
			lastMode = WHITE_BALL_MODE::SPIN;
		}
		else if (isInShootMode) {
			disableAll();
			isInShootMode = true;
			lastMode = WHITE_BALL_MODE::SHOOT;
		}
		else
		{
			isInFineAimMode = true;
		}
		break;
	case WHITE_BALL_MODE::SPIN:
		if (isInAimMode) {
			disableAll();
			isInAimMode = true;
			lastMode = WHITE_BALL_MODE::AIM;
		}
		else if (isInFineAimMode) {
			disableAll();
			isInFineAimMode = true;
			lastMode = WHITE_BALL_MODE::FINE_AIM;
		}
		else if (isInShootMode) {
			disableAll();
			isInShootMode = true;
			lastMode = WHITE_BALL_MODE::SHOOT;
		}
		else
		{
			isInSpinMode = true;
		}
		break;
	case WHITE_BALL_MODE::SHOOT:
		if (isInAimMode) {
			disableAll();
			isInAimMode = true;
			lastMode = WHITE_BALL_MODE::AIM;
		}
		else if (isInFineAimMode) {
			disableAll();
			isInFineAimMode = true;
			lastMode = WHITE_BALL_MODE::FINE_AIM;
		}
		else if (isInSpinMode) {
			disableAll();
			isInSpinMode = true;
			lastMode = WHITE_BALL_MODE::SPIN;
		}
		else
		{
			isInShootMode = true;
		}
		break;
	default:
		throw std::runtime_error("Not implemented checkbox!");
		break;
	}
}

void WhiteBallObject::Animate(float t, float dt)
{
	RoundManager& rm = RoundManager::GetInstance();


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
		XMFLOAT3 velocityF3 = camera->GetForward();
		velocityF3.y = 0.0;
		velocity = XMVector3Normalize(XMLoadFloat3(&velocityF3));

		//velocity = XMVectorSet(0, 0, -3, 0);
	}

	BallObject::Animate(t, dt);
}