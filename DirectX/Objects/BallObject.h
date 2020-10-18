#pragma once

#include "GameObject.h"
#include "Positions.h"

class PerspectiveCamera;

class BallObject : public GameObject
{
public:
	inline static float maxTessFactor = 32;

protected:
	XMVECTOR velocity;
	int point;

	XMVECTOR preferredPosition;

public:
	BallObject(ID3D11Device*, ID3D11DeviceContext*, int point, XMVECTOR = XMVectorSet(0, 0, 0, 0), XMVECTOR = BALL_SCALE, XMVECTOR = XMVectorSet(0, 0, 1, 0), float = 0.0f);
	~BallObject() = default;
	
	void SetVelocity(const XMVECTOR& velocity);
	XMVECTOR GetVelocity() const;
	XMVECTOR GetPreferredPosition() const;

	int GetPoint() const noexcept { return point; }

	bool IsMovingOutsideViewFrustum(Camera* camera) const;

	void Render(ID3D11DeviceContext*, Pipeline*, Camera* = nullptr, Light* = nullptr) override;
	void RenderToShadowMap(ID3D11DeviceContext*, Pipeline*, Light*) override;
	void Animate(float t, float dt) override;
};

