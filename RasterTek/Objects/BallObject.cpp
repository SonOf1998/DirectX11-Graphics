#include "pch.h"
#include "BallObject.h"

#include "Camera.h"
#include "CBufferDataType.h"
#include "QuadGeometry.h"
#include "Pipeline.h"
#include "Mesh.h"
#include "Light.h"

BallObject::BallObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMVECTOR position, XMVECTOR scale, XMVECTOR rotationAxis, float rotationAngle) : GameObject(device, deviceContext, position, scale, rotationAxis, rotationAngle)
{

}

void BallObject::Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera /* nullptr */, Light* light/* nullptr */)
{
	XMMATRIX viewProj = XMMatrixIdentity();
	if (camera != nullptr)
	{
		camera->Update();
		viewProj = camera->GetViewProjMatrix();
	}

	MVPMInv mvpminv;
	mvpminv.GetData().model = Transpose(modelMatrix);
	mvpminv.GetData().viewProj = Transpose(viewProj);
	mvpminv.GetData().modelInv = Inverse(modelMatrix);

	CameraCBuffer cameraCBuffer;
	cameraCBuffer.GetData().position = camera->GetPosition();

	Centroid centroid;
	centroid.GetData().position = position;

	pipeline->SetSampler(FILTERING::NEAREST, 0);

	pipeline->SetCBuffer(&cameraCBuffer, CBUFFER_LOCATION::HULL_SHADER_CBUFFER);
	pipeline->SetCBuffer(&centroid, CBUFFER_LOCATION::HULL_SHADER_CBUFFER);
	pipeline->SetCBuffer(&mvpminv, CBUFFER_LOCATION::DOMAIN_SHADER_CBUFFER);
	pipeline->SetCBuffer(&cameraCBuffer, CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER);

	for (auto& mesh : meshes)
	{
		pipeline->SetTexture(mesh->GetTexture());
		pipeline->SetCBuffer(mesh->GetMaterial(), CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER);
		mesh->GetGeometry()->Draw(deviceContext, DRAW_OPTIONS::AS_PATCH);
	}
}

void BallObject::RenderToShadowMap(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Light* light)
{
	DirLightVP dirLightVP;
	dirLightVP.GetData().viewProj = Transpose(light->GetViewProjMatrix());

	M m;
	m.GetData().model = Transpose(modelMatrix);
	pipeline->SetCBuffer(&dirLightVP, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);
	pipeline->SetCBuffer(&m, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);

	pipeline->Use();
	for (auto& mesh : meshes)
	{
		mesh->GetGeometry()->Draw(deviceContext);
	}
}

void BallObject::Animate(float t, float dt)
{
	//modelMatrix = XMMatrixScalingFromVector(scale) * XMMatrixRotationAxis(XMVectorSet(1, 0, -1, 0), -XM_PI / 2 * t) * XMMatrixTranslationFromVector(position);
}
