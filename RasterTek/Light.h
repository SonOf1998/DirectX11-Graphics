#pragma once
class Light
{
	// w = 0 means directional light
	// w = 1 defines exact location of the light emitter
	XMFLOAT4 position;
	XMFLOAT3 radiance;

protected:
	XMMATRIX viewProj;

public:
	Light(const XMFLOAT4& position, const XMFLOAT3& radiance);
	virtual ~Light() = default;

	XMMATRIX GetViewProjMatrix() const;
};

