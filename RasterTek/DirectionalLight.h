#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const XMFLOAT3& lightDir, const XMFLOAT3& radiance);
	~DirectionalLight() = default;
};

