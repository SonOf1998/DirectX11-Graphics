#pragma once

#include "GameObject.h"
#include "Positions.h"

class BallObject : public GameObject
{

protected:
	XMVECTOR velocity;
	short point;

public:
	BallObject(ID3D11Device*, ID3D11DeviceContext*, XMVECTOR = XMVectorSet(0, 0, 0, 0), XMVECTOR = BALL_SCALE, XMVECTOR = XMVectorSet(0, 0, 1, 0), float = 0.0f);
	~BallObject() = default;
	
	void SetVelocity(const XMVECTOR& velocity);
	XMVECTOR GetVelocity() const;

	void SetPoint(short pt) { point = pt; }
	short GetPoint() const noexcept { return point; }

	void Render(ID3D11DeviceContext*, Pipeline*, Camera* = nullptr, Light* = nullptr) override;
	void RenderToShadowMap(ID3D11DeviceContext*, Pipeline*, Light*) override;
	void Animate(float t, float dt) override;
};

