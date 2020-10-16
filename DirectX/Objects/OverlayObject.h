#pragma once

// from DirectX
#include "GameObject.h"

// from SnookerLogic
#include "RoundManager.h"

enum OVERLAY_SPOT
{
	OVERLAY_TARGET,
	OVERLAY_SPIN,
	OVERLAY_POT1,
	OVERLAY_POT2,
	OVERLAY_POT3,
	OVERLAY_POT4,
	OVERLAY_POT5,
	OVERLAY_POT6
};

class OverlayObject : public GameObject
{
	float ttl;		// time to live
	BALL ball;
	OVERLAY_SPOT overlaySpot;
	bool needsUpdate = false;


public:
	OverlayObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float ttl, BALL ball, OVERLAY_SPOT overlaySpot);
	~OverlayObject() = default;

	bool StillLiving() const noexcept;
	void Kill()				 noexcept;
	void MarkForUpdate()	 noexcept;
	bool IsTargetBallOverlay();
	void SetTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BALL targetBall);

	void Render(ID3D11DeviceContext*, Pipeline*, Camera* = nullptr, Light* = nullptr) override;
	void Animate(float t, float dt) override;
};

