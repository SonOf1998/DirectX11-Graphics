#pragma once

#include "Renderable.h"

enum BALL;

class OverlayObject;
class SpinOverlayObject;
class OrthographicCamera;

class OverlaySet : public Renderable
{
	std::deque<std::unique_ptr<OverlayObject>> overlays;
	std::unique_ptr<SpinOverlayObject> spinOverlay;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

public:
	OverlaySet(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~OverlaySet();

	void AddOverlayItem(std::unique_ptr<OverlayObject>&& overlayItem);
	void KillOverlayWithTTL();
	void ChangeTarget(BALL targetBall);
	void MarkForUpdate();

	virtual void Render(ID3D11DeviceContext* device, Pipeline* pipeline, Camera* camera = nullptr, Light* light = nullptr) override;
	virtual void Animate(float t, float dt) override;
};

