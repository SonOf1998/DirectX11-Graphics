#pragma once
#include "BallObject.h"

class WhiteBallObject : public BallObject
{

public:
	using BallObject::BallObject;

	void Animate(float, float) override;
};

