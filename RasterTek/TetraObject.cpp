#include "pch.h"
#include "TetraObject.h"

#include "Camera.h"
#include "Pipeline.h"

TetraObject::TetraObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMVECTOR translate, XMVECTOR scale, XMVECTOR rotationAxis, float rotationAngle) : GameObject(device, deviceContext, translate, scale, rotationAxis, rotationAngle)
{
}

void TetraObject::Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera, Light* /* nullptr */)
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

void TetraObject::Animate(float t, float dt)
{

}


