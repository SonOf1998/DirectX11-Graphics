#pragma once

class Camera;
class Light;
class Pipeline;

class Renderable
{
public:
	virtual ~Renderable() = default;

	virtual void Render(ID3D11DeviceContext* device, Pipeline* pipeline, Camera* camera = nullptr, Light* light = nullptr) = 0;
	virtual void RenderToShadowMap(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Light* light);
	virtual void Animate(float t, float dt) = 0;
};