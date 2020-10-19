#include "pch.h"
#include "ArrowSet.h"

// from DirectX
#include "BallSet.h"
#include "PerspectiveCamera.h"
#include "Pipeline.h"

// from SnookerLogic
#include "RoundManager.h"

ArrowSet::ArrowSet(ID3D11Device* device, ID3D11DeviceContext* deviceContext, PerspectiveCamera* camera, BallSet* ballSet) : ballSet(ballSet), camera(camera)
{
	int j = 0;
	for (int i = YELLOW; i <= BLACK; ++i)
	{
		colors[j++] = ballSet->GetColor((TARGET)i);
	}

	for (int i = 0; i < 6; ++i)
	{
		arrows[i] = std::make_unique<ArrowObject>(device, deviceContext);
	}
}

void ArrowSet::Render(ID3D11DeviceContext* deviceContext, Pipeline * pipeline, Camera * camera, Light * light)
{
	RoundManager& rm = RoundManager::GetInstance();
	if (rm.IsInNominateMode())
	{
		for (uint i = 0; i < 6; ++i)
		{
			arrows[i]->Render(deviceContext, pipeline, camera, light);
		}
	}
}

void ArrowSet::Animate(float t, float dt)
{
	RoundManager& rm = RoundManager::GetInstance();
	if (rm.IsInNominateMode())
	{

	}
}

ArrowSet::ArrowObject::ArrowObject(ID3D11Device* device, ID3D11DeviceContext * deviceContext) : GameObject()
{
	// Find and load a model!
}

void ArrowSet::ArrowObject::Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera, Light*)
{
	XMMATRIX proj = camera->GetProjMatrix();
	Proj p;
	p.GetData().proj = Transpose(proj);

	M m;
	m.GetData().model = Transpose(modelMatrix);

	BB bb;
	bb.GetData().rightVector = reinterpret_cast<PerspectiveCamera*>(camera)->GetRightVector();
	bb.GetData().upVector = reinterpret_cast<PerspectiveCamera*>(camera)->GetUpVector();

	pipeline->SetSampler(FILTERING::NEAREST, 0);
	pipeline->SetCBuffer(&m, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);
	pipeline->SetCBuffer(&p, CBUFFER_LOCATION::GEOMETRY_SHADER_CBUFFER);
	pipeline->SetCBuffer(&bb, CBUFFER_LOCATION::GEOMETRY_SHADER_CBUFFER);

	pipeline->SetTexture(meshes[0]->GetTexture());
	meshes[0]->GetGeometry()->Draw(deviceContext, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void ArrowSet::ArrowObject::Animate(float t, float dt)
{
	// intentionally left empty
}
