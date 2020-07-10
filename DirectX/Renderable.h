#pragma once
#include "Animatable.h"

class Camera;
class Light;
class Pipeline;

class Renderable : public Animatable
{
public:
	virtual ~Renderable() = default;

	virtual void Render(ID3D11DeviceContext* device, Pipeline* pipeline, Camera* camera = nullptr, Light* light = nullptr) = 0;
	virtual void RenderToShadowMap(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Light* light);
};