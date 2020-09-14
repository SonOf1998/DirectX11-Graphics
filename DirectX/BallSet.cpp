#include "pch.h"
#include "BallSet.h"

// from DirectX
#include "AssimpModel.h"
#include "BallObject.h"
#include "CollisionManager.h"
#include "Resources.h"
#include "Positions.h"
#include "WhiteBallObject.h"

// from SnookerLogic
#include "RoundManager.h"

// from Sounds
#include "SoundManager.h"

BallSet::BallSet(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// snooker balls - common resources (shape, shininess, colors)
	std::shared_ptr<Geometry> ballGeometry = std::make_shared<AssimpModel<P>>(device, LOW_QUALITY_SPHERE_MODEL);
	std::shared_ptr<Material> ballMaterial = std::make_shared<Material>(XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(1, 1, 1), 70.0f);
	std::shared_ptr<Texture>  redBallTexture	= std::make_shared<Texture>(device, deviceContext, RED_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  yellowBallTexture = std::make_shared<Texture>(device, deviceContext, YELLOW_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  greenBallTexture	= std::make_shared<Texture>(device, deviceContext, GREEN_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  brownBallTexture	= std::make_shared<Texture>(device, deviceContext, BROWN_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  blueBallTexture	= std::make_shared<Texture>(device, deviceContext, BLUE_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  pinkBallTexture	= std::make_shared<Texture>(device, deviceContext, PINK_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  blackBallTexture	= std::make_shared<Texture>(device, deviceContext, BLACK_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  whiteBallTexture	= std::make_shared<Texture>(device, deviceContext, WHITE_BALL_TEXTURE, 0);

	Mesh ballMesh(ballGeometry, ballMaterial);

	// WHITE BALL
	std::unique_ptr<BallObject> whiteBall = std::make_unique<WhiteBallObject>(device, deviceContext, WHITE_BALL_PREFERRED_POS);
	whiteBall->SetPoint(-4);
	ballMesh.SetTexture(whiteBallTexture);
	whiteBall->CopyAndAddMesh(ballMesh);
	balls.push_back(std::move(whiteBall));

	// YELLOW BALL
	std::unique_ptr<BallObject> yellowBall = std::make_unique<BallObject>(device, deviceContext, YELLOW_BALL_POS);
	yellowBall->SetPoint(2);
	ballMesh.SetTexture(yellowBallTexture);
	yellowBall->CopyAndAddMesh(ballMesh);
	balls.push_back(std::move(yellowBall));
	
	// GREEN BALL
	std::unique_ptr<BallObject> greenBall = std::make_unique<BallObject>(device, deviceContext, GREEN_BALL_POS);
	greenBall->SetPoint(3);
	ballMesh.SetTexture(greenBallTexture);
	greenBall->CopyAndAddMesh(ballMesh);
	balls.push_back(std::move(greenBall));
	
	// BROWN BALL
	std::unique_ptr<BallObject> brownBall = std::make_unique<BallObject>(device, deviceContext, BROWN_BALL_POS);
	brownBall->SetPoint(4);
	ballMesh.SetTexture(brownBallTexture);
	brownBall->CopyAndAddMesh(ballMesh);
	balls.push_back(std::move(brownBall));
	
	// BLUE BALL
	std::unique_ptr<BallObject> blueBall = std::make_unique<BallObject>(device, deviceContext, BLUE_BALL_POS);
	blueBall->SetPoint(5);
	ballMesh.SetTexture(blueBallTexture);
	blueBall->CopyAndAddMesh(ballMesh);
	balls.push_back(std::move(blueBall));
	
	// PINK BALL
	std::unique_ptr<BallObject> pinkBall = std::make_unique<BallObject>(device, deviceContext, PINK_BALL_POS);
	pinkBall->SetPoint(6);
	ballMesh.SetTexture(pinkBallTexture);
	pinkBall->CopyAndAddMesh(ballMesh);
	balls.push_back(std::move(pinkBall));
	
	// BLACK BALL
	std::unique_ptr<BallObject> blackBall = std::make_unique<BallObject>(device, deviceContext, BLACK_BALL_POS);
	blackBall->SetPoint(7);
	ballMesh.SetTexture(blackBallTexture);
	blackBall->CopyAndAddMesh(ballMesh);
	balls.push_back(std::move(blackBall));
	
	// RED BALLS
	XMFLOAT4 firstRedTranslation(0, BALL_POS_Y, PINK_BALL_Z - 2 * BALL_RADIUS, 0.0f);
	float deltaTranslate = 2 * BALL_RADIUS + 0.003f;	// added epsilon, so it will not report intersection (collision) right after launching
	for (int i = 0; i < 5; ++i)
	{
		float x0 = firstRedTranslation.x - ((i % 2) / 2.0f) * deltaTranslate;
		for (int j = 0; j <= i; ++j)
		{
			XMFLOAT4 translate = firstRedTranslation;
			translate.z -= i * deltaTranslate * 0.85f;
			translate.x = x0 + powf(-1, j + 1.0f) * j * deltaTranslate;
			x0 = translate.x;
	
			std::unique_ptr<BallObject> redBall = std::make_unique<BallObject>(device, deviceContext, XMLoadFloat4(&translate));
			redBall->SetPoint(1);
			ballMesh.SetTexture(redBallTexture);
			redBall->CopyAndAddMesh(ballMesh);
			balls.push_back(std::move(redBall));
		}
	}
}

BallSet::~BallSet() = default;

void BallSet::Render(ID3D11DeviceContext* device, Pipeline* pipeline, Camera* camera /* = nullptr */, Light* light /* = nullptr */)
{
	for (const auto& ball : balls)
	{
		ball->Render(device, pipeline, camera, light);
	}
}

void BallSet::RenderToShadowMap(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Light* light)
{
	for (const auto& ball : balls)
	{
		ball->RenderToShadowMap(deviceContext, pipeline, light);
	}
}

void BallSet::Animate(float t, float dt)
{
	RoundManager& rm = RoundManager::GetInstance();
	bool hadHit = false;

	// Collision with balls
	for (uint i = 0; i < balls.size(); ++i)
	{
		for (uint j = i + 1; j < balls.size(); ++j)
		{
			if (CollisionManager::Intersects(balls[i].get(), balls[j].get()))
			{
				// detecting failed snooker attempts in RoundManager::ManagePoint  
				hadHit = true;

				XMVECTOR normal = XMVector3Normalize(balls[i]->GetPosition() - balls[j]->GetPosition());
				XMVECTOR tangent = XMVectorSet(-XMVectorGetZ(normal), 0, XMVectorGetX(normal), 0);

				XMVECTOR v1n = XMVector3Dot(balls[i]->GetVelocity(), normal);	   // float scalars, only magnitudes
				XMVECTOR v1t = XMVector3Dot(balls[i]->GetVelocity(), tangent);	   // float scalars, only magnitudes
				XMVECTOR v2n = XMVector3Dot(balls[j]->GetVelocity(), normal);	   // float scalars, only magnitudes
				XMVECTOR v2t = XMVector3Dot(balls[j]->GetVelocity(), tangent);	   // float scalars, only magnitudes

				XMVECTOR v1n_new = XMVectorMultiply(v2n, normal);
				XMVECTOR v1t_new = XMVectorMultiply(v1t, tangent);
				XMVECTOR v2n_new = XMVectorMultiply(v1n, normal);
				XMVECTOR v2t_new = XMVectorMultiply(v2t, tangent);

				float centerDiff = Length(balls[i]->GetPosition() - balls[j]->GetPosition());
				float overlapDistance = 0.5f * (2 * BALL_RADIUS - centerDiff);

				balls[i]->SetPosition(balls[i]->GetPosition() + overlapDistance * normal);
				balls[j]->SetPosition(balls[j]->GetPosition() - overlapDistance * normal);

				balls[i]->SetVelocity(BALL_COLLISION_ENERGY_LOSS_FACTOR * (v1n_new + v1t_new));
				balls[j]->SetVelocity(BALL_COLLISION_ENERGY_LOSS_FACTOR * (v2n_new + v2t_new));

				SoundManager& sm = SoundManager::GetInstance();
				XMFLOAT3 collisionEffectCenter;
				XMStoreFloat3(&collisionEffectCenter, (balls[i]->GetPosition() + balls[j]->GetPosition()) / 2);
				sm.PlaySound(sm.GetBallBallCollisionSoundFileName(fabsf(XMVectorGetX(v1t - v2t))),
					collisionEffectCenter,
					reinterpret_cast<void*>(balls[i].get()),
					reinterpret_cast<void*>(balls[j].get())
				);
			}
		}
	}

	// Ball-Wall //
	for (uint i = 0; i < balls.size(); ++i)
	{
		XMFLOAT2 normal;
		XMFLOAT2 dummyNormal;
		if (CollisionManager::IntersectsWall(balls[i].get(), normal))
		{
			XMVECTOR normalV = XMVectorSet(normal.x, 0, normal.y, 0);
			XMVECTOR velocity = balls[i]->GetVelocity();

			do {
				balls[i]->SetPosition(balls[i]->GetPosition() - velocity * 0.01f * dt);
			} while (CollisionManager::IntersectsWall(balls[i].get(), dummyNormal));

			XMVECTOR reflectedVelocity = XMVector3Reflect(velocity, normalV);
			balls[i]->SetVelocity(reflectedVelocity * BALL_COLLISION_ENERGY_LOSS_FACTOR);
			SoundManager& sm = SoundManager::GetInstance();
			XMFLOAT3 collisionCenter;
			XMStoreFloat3(&collisionCenter, balls[i]->GetPosition() + XMVector3Normalize(velocity) * BALL_RADIUS);
			sm.PlaySound(sm.GetBallWallCollisionSoundFileName(XMVectorGetX(XMVector3Length(balls[i]->GetVelocity()))),
				collisionCenter,
				nullptr,
				nullptr
			);
		}
	}

	// Ball-Hole //
	for (uint i = 0; i < balls.size(); ++i)
	{
		if (CollisionManager::IntersectsHole(balls[i].get()))
		{
			rm.AddNewPottedBall(std::move(balls[i]));
			balls.erase(balls.begin() + i);

			SoundManager& sm = SoundManager::GetInstance();
			XMFLOAT3 collisionCenter;
			XMStoreFloat3(&collisionCenter, balls[i]->GetPosition());
			sm.PlaySound(sm.GetBallHoleCollosionSoundFileName(XMVectorGetX(XMVector3Length(balls[i]->GetVelocity()))),
				collisionCenter,
				reinterpret_cast<void*>(balls[i].get()),
				reinterpret_cast<void*>(balls[i].get())
			);
		}
	}

	bool stillMoving = false;
	for (const auto& ball : balls)
	{
		if (Length(ball->GetVelocity()) > 0.0f)
			stillMoving = true;
	}

	if (!stillMoving) // every ball is either in a static state or potted
	{
		rm.ManagePoints(hadHit);
	}


	for (const auto& ball : balls)
	{
		ball->Animate(t, dt);
	}
}
