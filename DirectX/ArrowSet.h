#pragma once

#include "GameObject.h"
#include "Renderable.h"

class BallObject;
class BallSet;
class Camera;
class PerspectiveCamera;
class Light;

class ArrowSet : public Renderable
{
	class ArrowObject : public GameObject
	{
		bool isHovered = false;

	public:
		ArrowObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		void Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera = nullptr, Light* light = nullptr) override;
		void Animate(float t, float dt) override;

		void SetHovered(bool hovered) noexcept { isHovered = hovered; }
		bool IsHovered() const noexcept { return isHovered;  }
	};
	
	BallSet* ballSet;
	PerspectiveCamera* camera;

	BallObject* colors[6];
	std::unique_ptr<ArrowObject> arrows[6];

public:
	ArrowSet(ID3D11Device* device, ID3D11DeviceContext* deviceContext, PerspectiveCamera* camera, BallSet* ballSet);
	~ArrowSet() = default;

	void Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera = nullptr, Light* light = nullptr) override;
	void Animate(float t, float dt) override;
};

