#include "pch.h"
#include "BallSet.h"

// from DirectX
#include "AssimpModel.h"
#include "BallObject.h"
#include "CollisionManager.h"
#include "CueObject.h"
#include "Resources.h"
#include "PerspectiveCamera.h"
#include "Positions.h"
#include "WhiteBallObject.h"

// from SnookerLogic
#include "RoundManager.h"

// from Sounds
#include "SoundManager.h"

BallSet::BallSet(ID3D11Device* device, ID3D11DeviceContext* deviceContext, PerspectiveCamera* camera, CueObject* cue) : device(device), deviceContext(deviceContext), camera(camera), cue(cue)
{
	InitTable();
}

BallSet::~BallSet() = default;

void BallSet::InitTable()
{
	balls.clear();

	// snooker balls - common resources (shape, shininess, colors)
	std::shared_ptr<Geometry> ballGeometry = std::make_shared<AssimpModel<P>>(device, LOW_QUALITY_SPHERE_MODEL);
	std::shared_ptr<Material> ballMaterial = std::make_shared<Material>(XMFLOAT3(0.3f, 0.3f, 0.3f), XMFLOAT3(0.8f, 0.8f, 0.8f), 70.0f);
	std::shared_ptr<Texture>  redBallTexture = std::make_shared<Texture>(device, deviceContext, RED_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  yellowBallTexture = std::make_shared<Texture>(device, deviceContext, YELLOW_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  greenBallTexture = std::make_shared<Texture>(device, deviceContext, GREEN_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  brownBallTexture = std::make_shared<Texture>(device, deviceContext, BROWN_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  blueBallTexture = std::make_shared<Texture>(device, deviceContext, BLUE_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  pinkBallTexture = std::make_shared<Texture>(device, deviceContext, PINK_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  blackBallTexture = std::make_shared<Texture>(device, deviceContext, BLACK_BALL_TEXTURE, 0);
	std::shared_ptr<Texture>  whiteBallTexture = std::make_shared<Texture>(device, deviceContext, WHITE_BALL_TEXTURE, 0);

	Mesh ballMesh(ballGeometry, ballMaterial);

	// YELLOW BALL
	std::unique_ptr<BallObject> yellowBall = std::make_unique<BallObject>(device, deviceContext, 2, YELLOW_BALL_POS);
	ballMesh.SetTexture(yellowBallTexture);
	yellowBall->CopyAndAddMesh(ballMesh);
	balls.push_back(std::move(yellowBall));

	// GREEN BALL
	std::unique_ptr<BallObject> greenBall = std::make_unique<BallObject>(device, deviceContext, 3, GREEN_BALL_POS);
	ballMesh.SetTexture(greenBallTexture);
	greenBall->CopyAndAddMesh(ballMesh);
	balls.push_back(std::move(greenBall));

	// BROWN BALL
	std::unique_ptr<BallObject> brownBall = std::make_unique<BallObject>(device, deviceContext, 4, BROWN_BALL_POS);
	ballMesh.SetTexture(brownBallTexture);
	brownBall->CopyAndAddMesh(ballMesh);
	balls.push_back(std::move(brownBall));

	// BLUE BALL
	std::unique_ptr<BallObject> blueBall = std::make_unique<BallObject>(device, deviceContext, 5, BLUE_BALL_POS);
	ballMesh.SetTexture(blueBallTexture);
	blueBall->CopyAndAddMesh(ballMesh);
	balls.push_back(std::move(blueBall));

	// PINK BALL
	std::unique_ptr<BallObject> pinkBall = std::make_unique<BallObject>(device, deviceContext, 6, PINK_BALL_POS);
	ballMesh.SetTexture(pinkBallTexture);
	pinkBall->CopyAndAddMesh(ballMesh);
	balls.push_back(std::move(pinkBall));

	// BLACK BALL
	std::unique_ptr<BallObject> blackBall = std::make_unique<BallObject>(device, deviceContext, 7, BLACK_BALL_POS);
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

			std::unique_ptr<BallObject> redBall = std::make_unique<BallObject>(device, deviceContext, 1, XMLoadFloat4(&translate));
			ballMesh.SetTexture(redBallTexture);
			redBall->CopyAndAddMesh(ballMesh);
			balls.push_back(std::move(redBall));
		}
	}

	// WHITE BALL
	std::unique_ptr<WhiteBallObject> whiteBall = std::make_unique<WhiteBallObject>(device, deviceContext, -4, camera, this, WHITE_BALL_PREFERRED_POS);
	ballMesh.SetTexture(whiteBallTexture);
	whiteBall->CopyAndAddMesh(ballMesh);
	whiteBallRef = whiteBall.get();
	cue->InitWhiteBall(whiteBall.get());
	balls.push_back(std::move(whiteBall));
}

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

std::map<int, XMVECTOR> preferredPositions = {
	{-4, WHITE_BALL_PREFERRED_POS},
	{2, YELLOW_BALL_POS},
	{3, GREEN_BALL_POS},
	{4, BROWN_BALL_POS},
	{5, BLUE_BALL_POS},
	{6, PINK_BALL_POS},
	{7, BLACK_BALL_POS}
};

void BallSet::Animate(float t, float dt)
{
	RoundManager& rm = RoundManager::GetInstance();

	// Collision with balls
	for (uint i = 0; i < balls.size(); ++i)
	{
		for (uint j = i + 1; j < balls.size(); ++j)
		{
			if (CollisionManager::Intersects(balls[i].get(), balls[j].get()))
			{
				// detecting failed snooker attempts in RoundManager::ManagePoint  
				// also it handles first hit fouls
				if (balls[i]->GetPoint() == -4)
				{
					rm.MemoFirstHit(balls[j].get());
				}
				else if (balls[j]->GetPoint() == -4)
				{
					rm.MemoFirstHit(balls[i].get());
				}
				
				// TODO spin!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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

			// TODO needs urgent refactor as it can freeze the game totally
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
			if (balls[i]->GetPoint() == -4)
				rm.SetWhiteDropped(true);

			XMFLOAT3 collisionCenter;
			float velocity = XMVectorGetX(XMVector3Length(balls[i]->GetVelocity()));
			balls[i]->SetVelocity(NULL_VELOCITY);
			XMStoreFloat3(&collisionCenter, balls[i]->GetPosition());

			rm.AddNewPottedBall(std::move(balls[i]));
			balls.erase(balls.begin() + i);

			SoundManager& sm = SoundManager::GetInstance();
			sm.PlaySound(sm.GetBallHoleCollisionSoundFileName(velocity),
				collisionCenter,
				nullptr,
				nullptr
			);
		}
	}

	bool stillMoving = false;
	for (const auto& ball : balls)
	{
		if (Length(ball->GetVelocity()) > 0.0f)
			stillMoving = true;
	}

	// The time to finish the round and handle the points
	// every ball is either in a static state or potted
	// and round is not yet administrated
	if (!stillMoving && rm.IsRoundGoing()) 
	{
		rm.ManagePoints(this);
		std::vector<std::unique_ptr<BallObject>> ballsToPutBack = rm.GetBallsToPutBack(this);

		struct BallReplacementStatus
		{
			uint index;
			bool placedBack;
		};

		std::vector<BallReplacementStatus> statuses;	

		for (uint i = 0; i < ballsToPutBack.size(); ++i)
		{
			std::unique_ptr<BallObject>& ball = ballsToPutBack[i];
			bool isWhite = ball->GetPoint() == -4;

			XMVECTOR placeBackPos = preferredPositions[ball->GetPoint()];
			bool placedBack = false;

			// as the first attempt, just try to replace the ball to its original spot
			// most of the time it will work
			if (!IsPlaceUsed(placeBackPos, true))
			{
				ball->SetPosition(placeBackPos);
				balls.push_back(std::move(ball));
				placedBack = true;
			}
			// if it didn't work out then memorize the indices
			// of the balls that aren't placed back yet
			else {
				statuses.push_back(BallReplacementStatus{ i, false });
			}
		}

		for (auto& status : statuses)
		{
			std::unique_ptr<BallObject>& ball = ballsToPutBack[status.index];
			bool isWhite = ball->GetPoint() == -4;

			bool placed = false;

			// try putting the balls back to the empty places of coloured balls
			for (uint i = 7; i >= 2 && !placed && !isWhite; --i)
			{
				XMVECTOR placeBackPos = preferredPositions[i];
				if (!IsPlaceUsed(placeBackPos, true))
				{
					ball->SetPosition(placeBackPos);
					balls.push_back(std::move(ball));
					placed = true;
				}
			}

			// if it fails then try to put as close to pink as possible
			// handling white is unique: we try every position on the D-shape's straight part
			if (!placed)
			{
				if (isWhite)
				{
					XMVECTOR placeBackPos = preferredPositions[-4];
					do {
						placeBackPos -= XMVectorSet(0.004f, 0, 0, 0);
					} while (IsPlaceUsed(placeBackPos, true));

					ball->SetPosition(placeBackPos);
					balls.push_back(std::move(ball));
				}
				else
				{
					XMVECTOR placeBackPos = preferredPositions[6];
					do {
						placeBackPos += XMVectorSet(0, 0, 0.004f, 0);
					} while (IsPlaceUsed(placeBackPos, true));

					ball->SetPosition(placeBackPos);
					balls.push_back(std::move(ball));
				}
			}
		}

		camera->SetWhiteBallPos(whiteBallRef->GetPosition());
		camera->SetTargetBallPos(GetClosestTargetBallToCueBall(whiteBallRef->GetPosition(), rm.GetTarget()));
		camera->GoAimMode();
	}

	for (const auto& ball : balls)
	{
		ball->Animate(t, dt);
	}
}

XMVECTOR BallSet::GetClosestColorToCueBall(const XMVECTOR& cueBallPos, TARGET* targetOutput) const
{
	BallObject* targetBallWithMinDist = nullptr;
	float minDist = std::numeric_limits<float>::max();

	for (uint i = 0; i < balls.size(); ++i)
	{
		if (balls[i]->GetPoint() < 2)
			continue;

		XMVECTOR dir = balls[i]->GetPosition() - cueBallPos;
		float minDistTemp = Length(balls[i]->GetPosition() - cueBallPos);
		if (targetBallWithMinDist == nullptr || minDistTemp < minDist)
		{
			bool obstacleFound = false;

			// looking for obstacles
			for (uint j = 0; j < balls.size(); ++j)
			{
				if (i == j || balls[j]->GetPoint() == -4)
					continue;

				// potential obstacle as it is closer to the cue ball as the target one
				float potentialObsDist = Length(balls[j]->GetPosition() - cueBallPos);
				if (potentialObsDist < minDistTemp)
				{
					XMFLOAT2 dir2D = XMFLOAT2(XMVectorGetX(dir), XMVectorGetZ(dir));
					XMFLOAT2 dir2DNormal = XMFLOAT2(dir2D.y, -dir2D.x);
					XMFLOAT2 targetPos2D = XMFLOAT2(XMVectorGetX(balls[i]->GetPosition()), XMVectorGetZ(balls[i]->GetPosition()));

					XMFLOAT2 whitePos2D = XMFLOAT2(XMVectorGetX(cueBallPos), XMVectorGetZ(cueBallPos));
					XMFLOAT2 potentialObsPos = XMFLOAT2(XMVectorGetX(balls[j]->GetPosition()), XMVectorGetZ(balls[j]->GetPosition()));

					// checking if obstacle completely blocks vision
					// this is done by checking the main direction
					// and allowing a little sticking out for the target ball
					float t = 0;
					while (Distance(whitePos2D + t * dir2D, whitePos2D) < potentialObsDist)
					{
						if (Distance(whitePos2D + t * dir2D, potentialObsPos) < 2 * BALL_RADIUS)
						{
							obstacleFound = true;
							break;
						}

						t += 0.005f;
					}

					if (obstacleFound)
					{
						break;
					}
				}
			}

			if (!obstacleFound)
			{
				minDist = Length(balls[i]->GetPosition() - cueBallPos);
				targetBallWithMinDist = balls[i].get();
			}
		}
	}

	if (targetBallWithMinDist == nullptr)
	{
		for (uint i = 0; i < balls.size(); ++i)
		{
			if (balls[i]->GetPoint() >= 2 && Length(balls[i]->GetPosition() - cueBallPos) < minDist)
			{
				minDist = Length(balls[i]->GetPosition() - cueBallPos);
				targetBallWithMinDist = balls[i].get();
			}
		}
	}

	*targetOutput = (TARGET)(targetBallWithMinDist->GetPoint() - 1);
	return targetBallWithMinDist->GetPosition();
}

XMVECTOR BallSet::GetClosestTargetBallToCueBall(const XMVECTOR& cueBallPos, TARGET target) const
{
	BallObject* targetBallWithMinDist = nullptr;
	float minDist = std::numeric_limits<float>::max();

	if (target == RED)
	{
		for (uint i = 0; i < balls.size(); ++i)
		{
			if (balls[i]->GetPoint() != 1)
				continue;

			XMVECTOR dir = balls[i]->GetPosition() - cueBallPos;
			float minDistTemp = Length(balls[i]->GetPosition() - cueBallPos);
			if (targetBallWithMinDist == nullptr || minDistTemp < minDist)
			{
				bool obstacleFound = false;

				// looking for obstacles
				for (uint j = 0; j < balls.size(); ++j)
				{
					if (i == j || balls[j]->GetPoint() == -4)
						continue;

					// potential obstacle as it is closer to the cue ball as the target one
					float potentialObsDist = Length(balls[j]->GetPosition() - cueBallPos);
					if (potentialObsDist < minDistTemp)
					{
						XMFLOAT2 dir2D = XMFLOAT2(XMVectorGetX(dir), XMVectorGetZ(dir));
						XMFLOAT2 dir2DNormal = XMFLOAT2(dir2D.y, -dir2D.x);
						XMFLOAT2 targetPos2D = XMFLOAT2(XMVectorGetX(balls[i]->GetPosition()), XMVectorGetZ(balls[i]->GetPosition()));
						
						XMFLOAT2 whitePos2D = XMFLOAT2(XMVectorGetX(cueBallPos), XMVectorGetZ(cueBallPos));
						XMFLOAT2 potentialObsPos = XMFLOAT2(XMVectorGetX(balls[j]->GetPosition()), XMVectorGetZ(balls[j]->GetPosition()));

						// checking if obstacle completely blocks vision
						// this is done by checking the main direction
						// and allowing a little sticking out for the target ball
						float t = 0;
						while (Distance(whitePos2D + t * dir2D, whitePos2D) < potentialObsDist)
						{
							if (Distance(whitePos2D + t * dir2D, potentialObsPos) < 2 * BALL_RADIUS)
							{
								obstacleFound = true;
								break;
							}

							t += 0.005f;
						}

						if (obstacleFound)
						{
							break;
						}
					}
				}

				if (!obstacleFound)
				{
					minDist = Length(balls[i]->GetPosition() - cueBallPos);
					targetBallWithMinDist = balls[i].get();
				}
			}			
		}
	}
	else /* COLORED BALLS */
	{
		for (uint i = 0; i < balls.size(); ++i)
		{
			if (balls[i]->GetPoint() == (target + 1))
			{
				return balls[i]->GetPosition();
			}
		}
	}

	if (targetBallWithMinDist == nullptr)
	{
		for (uint i = 0; i < balls.size(); ++i)
		{
			if (balls[i]->GetPoint() == 1 && Length(balls[i]->GetPosition() - cueBallPos) < minDist)
			{
				minDist = Length(balls[i]->GetPosition() - cueBallPos);
				targetBallWithMinDist = balls[i].get();
			}
		}
	}

	return targetBallWithMinDist->GetPosition();
}

XMVECTOR BallSet::GetWhiteBallPosition() const
{
	return whiteBallRef->GetPosition();
}

void BallSet::MakeWhiteMovable()
{
	whiteBallRef->SetPosition(WHITE_BALL_PREFERRED_POS);
	RoundManager& rm = RoundManager::GetInstance();
	rm.SetWhiteDropped(true);
	rm.SetWhitePlaced(true);
}

bool BallSet::IsPlaceUsed(const XMVECTOR& place, bool excludeWhite /* =false */) const
{
	for (auto& ball : balls)
	{
		if (excludeWhite && ball->GetPoint() == -4)
			continue;

		if (Length(place - ball->GetPosition()) + 0.00001f < 2 * BALL_RADIUS)
		{
			return true;
		}
	}

	return false;
}

bool BallSet::HasReds() const
{
	for (const auto& ball : balls)
	{
		if (ball->GetPoint() == 1)
		{
			return true;
		}
	}

	return false;
}

bool BallSet::HasLower(TARGET target) const
{
	// white ball excluded
	for (const auto& ball : balls)
	{
		if (ball->GetPoint() > 1 && ball->GetPoint() < target + 1)
		{
			return true;
		}
	}

	return false;
}