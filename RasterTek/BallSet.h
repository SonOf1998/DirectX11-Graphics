#pragma once
#include "Renderable.h"

class GameObject;

class BallSet : public Renderable
{
	std::vector<std::unique_ptr<GameObject>> balls;

public:
	BallSet(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~BallSet();

	virtual void Render(ID3D11DeviceContext* device, Pipeline* pipeline, Camera* camera = nullptr, Light* light = nullptr) override;
	virtual void RenderToShadowMap(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Light* light);
	virtual void Animate(float t, float dt) override;
};

