#pragma once

#include "GameObject.h"
#include "Renderable.h"

class BallObject;
class BallSet;
class Camera;
class PerspectiveCamera;
class Texture;
class Light;

class ArrowSet : public Renderable
{
	class ArrowObject : public GameObject
	{
		bool isCurrentlyHovered = false;
		std::shared_ptr<Texture> grayArrow;
		std::shared_ptr<Texture> blueArrow;

	public:
		ArrowObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		void Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera = nullptr, Light* light = nullptr) override;
		void Animate(float t, float dt) override;

		void MakeHovered(bool hovered);
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

