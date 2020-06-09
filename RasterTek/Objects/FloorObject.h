#pragma once
#include "GameObject.h"

class FloorObject : public GameObject
{

public:
	using GameObject::GameObject;

	void Render(ID3D11DeviceContext*, ShaderProgram*, Camera* = nullptr, Light* = nullptr) override;
	void RenderToShadowMap(ID3D11DeviceContext*, ShaderProgram*, Light*) override;
	void Animate(float, float) override;
};

