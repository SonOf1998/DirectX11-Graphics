#include "pch.h"
#include "Light.h"

Light::Light(const XMFLOAT4& position, const XMFLOAT3& radiance) : position(position), radiance(radiance)
{

}

XMMATRIX Light::GetViewProjMatrix() const
{
	return viewProj;
}
