#include "pch.h"
#include "FullScreenQuadObject.h"

#include "CBufferDataType.h"
#include "ShaderProgram.h"
#include "SimpleDXMath.h"
#include "Camera.h"
#include "Mesh.h"

void FullScreenQuadObject::Render(ID3D11DeviceContext* deviceContext, ShaderProgram* shaderProgram, Camera* camera)
{
	XMMATRIX viewProj = XMMatrixIdentity();
	if (camera != nullptr)
	{
		camera->Update();
		viewProj = camera->GetViewProjMatrix();
	}

	MVP mvp;
	mvp.GetData().model = Transpose(modelMatrix);
	mvp.GetData().viewProj = Transpose(viewProj);

	shaderProgram->SetSampler(FILTERING::ANISOTROPIC_X16, 0);
	shaderProgram->SetCBuffer(&mvp, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);

	shaderProgram->Use();

	for (auto& mesh : meshes)
	{
		mesh->GetGeometry()->Draw(deviceContext);
	}
}

void FullScreenQuadObject::Animate(float t, float dt)
{
}
