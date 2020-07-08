#include "pch.h"
#include "CollisionManager.h"

#include "BallObject.h"

bool CollisionManager::Intersects(BallObject* ball1, BallObject* ball2)
{
	float centerDiff = Length(ball1->GetPosition() - ball2->GetPosition());
	if (centerDiff <= 2 * BALL_RADIUS)
	{
		return true;
	}

	return false;
}
