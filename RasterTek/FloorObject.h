#pragma once
#include "GameObject.h"

class FloorObject : public GameObject
{

public:
	using GameObject::GameObject;

	void Render(ID3D11DeviceContext*, ShaderProgram*, Camera* = nullptr) override;
	void Animate(float, float) override;
};

