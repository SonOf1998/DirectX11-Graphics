#pragma once
class Animatable
{
public:
	Animatable();
	virtual ~Animatable() = default;

	virtual void Animate(float t, float dt);
};

