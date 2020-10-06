#pragma once
#include "GameObject.h"

#include "Positions.h"

class PerspectiveCamera;
class WhiteBallObject;

class CueObject : public GameObject
{
	PerspectiveCamera* camera;
	XMVECTOR orientation = XMVectorSet(0, 0, -1, 0);
	WhiteBallObject* whiteBall;

	// Cue distance factor: determines how far should we place the cue from the white ball on the -dir2DF vector. (dir2DF is normalized)
	// used when player is in shoot mode
	float cdf = CDF_DEFAULT_VAL;

public:
	CueObject(ID3D11Device*, ID3D11DeviceContext*, PerspectiveCamera* camera, XMVECTOR = XMVectorSet(0, 0, 0, 0), XMVECTOR = BALL_SCALE, XMVECTOR = XMVectorSet(0, 0, 1, 0), float = 0.0f);
	void InitWhiteBall(WhiteBallObject* whiteBall); // need to do separately asks for refactor!
	~CueObject() = default;

	void Render(ID3D11DeviceContext*, Pipeline*, Camera* = nullptr, Light* = nullptr) override;
	void RenderToShadowMap(ID3D11DeviceContext*, Pipeline*, Light*) override;
	void Animate(float t, float dt) override;

	void ResetCDF() noexcept;
};

