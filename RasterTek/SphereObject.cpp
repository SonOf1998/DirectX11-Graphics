#include "pch.h"
#include "SphereObject.h"

#include "Camera.h"
#include "CBufferDataType.h"
#include "QuadGeometry.h"
#include "ShaderProgram.h"
#include "SimpleDXMath.h"
#include "Mesh.h"

void SphereObject::Render(ID3D11DeviceContext* deviceContext, ShaderProgram* shaderProgram, Camera* camera)
{
	XMMATRIX viewProj = XMMatrixIdentity();
	if (camera != nullptr)
	{
		camera->Update();
		viewProj = camera->GetViewProjMatrix();
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

void SphereObject::Animate(float t, float st)
{
	modelMatrix = XMMatrixScalingFromVector(scale) * XMMatrixRotationAxis(XMVectorSet(1, 0, -1, 0), -XM_PI / 2 * t) * XMMatrixTranslationFromVector(position);
}
