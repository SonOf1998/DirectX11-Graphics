#pragma once
#include "Camera.h"

class OrthographicCamera : public Camera
{
	bool needsUpdate = false;

public:
	OrthographicCamera();
	~OrthographicCamera() = default;

	void MarkForUpdate()			noexcept;
	void Animate(float t, float dt) override;
};

