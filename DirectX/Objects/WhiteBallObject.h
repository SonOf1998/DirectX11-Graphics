#pragma once
#include "BallObject.h"

class BallSet;
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
	BallSet* ballSet;

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
	WhiteBallObject(ID3D11Device*, ID3D11DeviceContext*, int point, PerspectiveCamera* camera, BallSet* ballSet,
					XMVECTOR = XMVectorSet(0, 0, 0, 0), XMVECTOR = BALL_SCALE, XMVECTOR = XMVectorSet(0, 0, 1, 0), float = 0.0f);
	~WhiteBallObject() = default;


	void Animate(float t, float dt) override;
};

