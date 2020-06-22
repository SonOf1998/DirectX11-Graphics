#pragma once
#include "Camera.h"

class OrtographicCamera : public Camera
{

public:
	OrtographicCamera(const XMVECTOR& position);
	~OrtographicCamera() = default;
};

