#pragma once

class BallObject;

class CollisionManager
{
public:
	CollisionManager() = delete;
	~CollisionManager() = default;

	static bool Intersects(BallObject* ball1, BallObject* ball2);
};

