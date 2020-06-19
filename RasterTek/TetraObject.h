#pragma once
#include "GameObject.h"

class TetraObject : public GameObject
{
public:

	TetraObject(ID3D11Device*, ID3D11DeviceContext*, XMVECTOR = XMVectorSet(0, 0, 0, 0), XMVECTOR = BALL_SCALE, XMVECTOR = XMVectorSet(0, 0, 1, 0), float = 0.0f);
	~TetraObject() = default;

	void Render(ID3D11DeviceContext*, Pipeline*, Camera* = nullptr, Light* = nullptr) override;
	void Animate(float t, float dt) override;
};

