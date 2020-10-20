#include "pch.h"
#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(const XMFLOAT3& lightDir, const XMFLOAT3 & radiance) : Light(XMFLOAT4(lightDir.x, lightDir.y, lightDir.z, 0), radiance)
{
	XMMATRIX view = XMMatrixLookAtRH(XMVectorSet(lightDir.x, lightDir.y, lightDir.z, 1), XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 0, -1, 0));
	XMMATRIX projection = XMMatrixOrthographicRH(27, 27, 0, 30);

	viewProj = view * projection;
}
