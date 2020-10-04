#include "pch.h"
#include "WhiteBallObject.h"

// from DirectX
#include "Graphics.h"
#include "BallSet.h"
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

WhiteBallObject::WhiteBallObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int point, PerspectiveCamera* camera,
								 BallSet* ballSet, XMVECTOR pos, XMVECTOR scale, XMVECTOR rotAxis, float rotAngle) :
	BallObject(device, deviceContext, point, pos, scale, rotAxis, rotAngle),
	camera(camera),
	ballSet(ballSet)
{
	RoundManager& rm = RoundManager::GetInstance();

	camera->SetWhiteBallPos(pos);
	camera->SetTargetBallPos(ballSet->GetClosestTargetBallToCueBall(pos, rm.GetTarget()));
	camera->GoAimMode();
}

void WhiteBallObject::Animate(float t, float dt)
{
	RoundManager& rm = RoundManager::GetInstance();
	//	Logger::print(std::to_string(XMVectorGetX(position)) + " , " + std::to_string(XMVectorGetZ(position)));ww

	// Picking technique
	if (!rm.IsWhitePlaced())
	{
		POINT cursorPos = InputClass::GetCursorPosition();

		// cursor pos to NDC
		float posX = 2 * (cursorPos.x / static_cast<float>(Graphics::screenWidth) - 0.5f);
		float posY = 2 * (-cursorPos.y / static_cast<float>(Graphics::screenHeight) + 0.5f);
		
		XMVECTOR farPlanePick = XMVectorSet(posX, posY, 1, 1);
		XMVECTOR nearPlanePick = XMVectorSet(posX, posY, 0, 1);

		farPlanePick = farPlanePick * camera->GetViewProjMatrixInv();
		nearPlanePick = nearPlanePick * camera->GetViewProjMatrixInv();

		XMFLOAT4 fpp4, npp4;
		XMStoreFloat4(&fpp4, farPlanePick);
		XMStoreFloat4(&npp4, nearPlanePick);

		XMFLOAT3 fpp = XMFLOAT3(fpp4.x / fpp4.w, fpp4.y / fpp4.w, fpp4.z / fpp4.w);
		XMFLOAT3 npp = XMFLOAT3(npp4.x / npp4.w, npp4.y / npp4.w, npp4.z / npp4.w);

		XMFLOAT3 dir = XMFLOAT3(fpp.x - npp.x, fpp.y - npp.y, fpp.z - npp.z);
		XMFLOAT3 s = npp;

		float t_ray = (BALL_POS_Y - s.y) / dir.y;
		float x_ray = s.x + t_ray * dir.x;
		float z_ray = s.z + t_ray * dir.z;
		
		XMVECTOR newPos = XMVectorSet(x_ray, BALL_POS_Y, z_ray, 0);
		if (x_ray * x_ray + (z_ray - DZ) * (z_ray - DZ) <= 1.0f && z_ray > DZ && !ballSet->IsPlaceUsed(newPos, true))
		{
			position = newPos;
		}

		if (InputClass::LeftMBDown())
		{
			rm.SetWhitePlaced(true);
			camera->SetWhiteBallPos(position);
			camera->SetTargetBallPos(ballSet->GetClosestTargetBallToCueBall(position, rm.GetTarget()));
			camera->GoAimMode();
		}
	}

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

	//  Logger::print(std::to_string(XMVectorGetX(position)) + " " + std::to_string(XMVectorGetZ(position)));

	if (InputClass::IsKeyDown(VK_SPACE))
	{
		XMFLOAT3 velocityF3 = camera->GetForward();
		velocityF3.y = 0.0;
		velocity = XMVector3Normalize(XMLoadFloat3(&velocityF3));

		//velocity = XMVectorSet(0, 0, -3, 0);
	}

	BallObject::Animate(t, dt);
}