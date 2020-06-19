#include "pch.h"
#include "FullScreenQuadObject.h"

#include "CBufferDataType.h"
#include "Pipeline.h"
#include "SimpleDXMath.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"

void FullScreenQuadObject::Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera /* nullptr */, Light* light /* nullptr */)
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
		pipeline->SetCBuffer(&dirLightVP, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);
	}

	MVP mvp;
	mvp.GetData().model = Transpose(modelMatrix);
	mvp.GetData().viewProj = Transpose(viewProj);

	pipeline->SetSampler(FILTERING::ANISOTROPIC_X16, 0);
	pipeline->SetCBuffer(&mvp, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);

	pipeline->Use();

	for (auto& mesh : meshes)
	{
		mesh->GetGeometry()->Draw(deviceContext);
	}
}

void FullScreenQuadObject::Animate(float t, float dt)
{
}
