#pragma once
#include "BallObject.h"

class PerspectiveCamera;

class WhiteBallObject : public BallObject
{
	PerspectiveCamera* camera;

public:
	using BallObject::BallObject;

	void Animate(float t, float dt) override;
};

