#include "pch.h"
#include "BallObject.h"

#include "Camera.h"
#include "CBufferDataType.h"
#include "QuadGeometry.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Light.h"

void BallObject::Render(ID3D11DeviceContext* deviceContext, ShaderProgram* shaderProgram, Camera* camera /* nullptr */, Light* light/* nullptr */)
{
	XMMATRIX viewProj = XMMatrixIdentity();
	if (camera != nullptr)
	{
		camera->Update();
		viewProj = camera->GetViewProjMatrix();
	}
	
	if (light != nullptr)
	{
		DirLightVP dirLightVP;
		dirLightVP.GetData().viewProj = Transpose(light->GetViewProjMatrix());
		shaderProgram->SetCBuffer(&dirLightVP, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);
	}

	MVPMInv mpvminv;
	mpvminv.GetData().model = Transpose(modelMatrix);
	mpvminv.GetData().viewProj = Transpose(viewProj);
	mpvminv.GetData().modelInv = Inverse(modelMatrix);

	shaderProgram->SetSampler(FILTERING::NEAREST, 0);
	shaderProgram->SetCBuffer(&mpvminv, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);


	CameraCBuffer cameraCBuffer = camera->GetCBuffer();
	shaderProgram->SetCBuffer(&cameraCBuffer, CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER);

	for (auto& mesh : meshes)
	{
		shaderProgram->SetTexture(mesh->GetTexture());
		shaderProgram->Use();
		shaderProgram->SetCBuffer(mesh->GetMaterial(), CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER);
		mesh->GetGeometry()->Draw(deviceContext);
	}
}

void BallObject::RenderToShadowMap(ID3D11DeviceContext* deviceContext, ShaderProgram* shaderProgram, Light* light)
{
	DirLightVP dirLightVP;
	dirLightVP.GetData().viewProj = Transpose(light->GetViewProjMatrix());

	M m;
	m.GetData().model = Transpose(modelMatrix);
	shaderProgram->SetCBuffer(&dirLightVP, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);
	shaderProgram->SetCBuffer(&m, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);

	shaderProgram->Use();
	for (auto& mesh : meshes)
	{
		mesh->GetGeometry()->Draw(deviceContext);
	}
}

void BallObject::Animate(float t, float st)
{
	//modelMatrix = XMMatrixScalingFromVector(scale) * XMMatrixRotationAxis(XMVectorSet(1, 0, -1, 0), -XM_PI / 2 * t) * XMMatrixTranslationFromVector(position);
}
