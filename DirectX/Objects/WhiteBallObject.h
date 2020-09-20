#pragma once
#include "BallObject.h"

class PerspectiveCamera;

enum class WHITE_BALL_MODE
{
	AIM,
	FINE_AIM,
	SPIN,
	SHOOT
};

class WhiteBallObject : public BallObject
{
	PerspectiveCamera* camera;

public:
	// statics as std::function is not supported
	// in ImGui's checkbox implementation
	inline static bool isInAimMode		= true;
	inline static bool isInFineAimMode	= false;
	inline static bool isInSpinMode		= false;
	inline static bool isInShootMode	= false;
private:
	inline static WHITE_BALL_MODE lastMode = WHITE_BALL_MODE::AIM;
public:
	static void SwitchModes();


public:
	using BallObject::BallObject;

	void Animate(float t, float dt) override;
};

