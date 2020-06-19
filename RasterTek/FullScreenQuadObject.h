#pragma once
#include "GameObject.h"
class FullScreenQuadObject : public GameObject
{
public:
	using GameObject::GameObject;

	virtual void Render(ID3D11DeviceContext*, Pipeline*, Camera* = nullptr, Light* = nullptr) override;
	virtual void Animate(float t, float dt) override;
};

