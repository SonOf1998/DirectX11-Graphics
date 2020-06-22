#pragma once
#include "GameObject.h"

class ArmchairObject : public GameObject
{

public:

	ArmchairObject(ID3D11Device*, ID3D11DeviceContext*, XMVECTOR = XMVectorSet(0, -0.8f, 10, 0), XMVECTOR = XMVectorSet(1,1,1,1), XMVECTOR = XMVectorSet(0, 1, 0, 0), float = -XM_PI);
	~ArmchairObject() = default;

	void Render(ID3D11DeviceContext*, Pipeline*, Camera* = nullptr, Light* = nullptr) override;
	void RenderToShadowMap(ID3D11DeviceContext*, Pipeline*, Light*) override;
	void Animate(float t, float dt) override;
};

