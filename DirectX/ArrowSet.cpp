#include "pch.h"
#include "ArrowSet.h"

// from DirectX
#include "AssimpModel.h"
#include "BallSet.h"
#include "PerspectiveCamera.h"
#include "Pipeline.h"
#include "Resources.h"
#include "Texture.h"

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
		for (int i = 0; i < 6; ++i)
		{
			arrows[i]->SetPosition(colors[i]->GetPosition() + XMVectorSet(0, ARROW_DY, 0, 0));
			arrows[i]->Animate(t, dt);
		}
	}
}

ArrowSet::ArrowObject::ArrowObject(ID3D11Device* device, ID3D11DeviceContext * deviceContext) : GameObject()
{
	// single point
	// will get extended in the matching geometry shader
	std::shared_ptr<Geometry> ptGeometry = std::make_shared<AssimpModel<P>>(device, POINT_MODEL, 0, false);

	grayArrow = std::make_unique<Texture>(device, deviceContext, GRAY_ARROW_TEXTURE, 0);
	blueArrow = std::make_unique<Texture>(device, deviceContext, BLUE_ARROW_TEXTURE, 0);

	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(ptGeometry);
	mesh->SetTexture(grayArrow);

	AddMesh(std::move(mesh));
}

void ArrowSet::ArrowObject::Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera, Light*)
{
	XMMATRIX viewProj = camera->GetViewProjMatrix();
	VP vp;
	vp.GetData().viewProj = Transpose(viewProj);

	M m;
	m.GetData().model = Transpose(modelMatrix);

	BB bb;
	bb.GetData().rightVector = XMVectorSetY(reinterpret_cast<PerspectiveCamera*>(camera)->GetRightVector(), 0);
	bb.GetData().upVector = XMVectorSetZ(XMVectorSetX(reinterpret_cast<PerspectiveCamera*>(camera)->GetUpVector(), 0), 0);

	pipeline->SetSampler(FILTERING::NEAREST, 0);
	pipeline->SetCBuffer(&m, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);
	pipeline->SetCBuffer(&vp, CBUFFER_LOCATION::GEOMETRY_SHADER_CBUFFER);
	pipeline->SetCBuffer(&bb, CBUFFER_LOCATION::GEOMETRY_SHADER_CBUFFER);

	pipeline->SetTexture(meshes[0]->GetTexture());
	meshes[0]->GetGeometry()->DrawUnindexed(deviceContext, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void ArrowSet::ArrowObject::Animate(float t, float dt)
{
	modelMatrix = XMMatrixTranslationFromVector(position);
}

void ArrowSet::ArrowObject::MakeHovered(bool hovered)
{
	if (hovered && !isCurrentlyHovered)
	{
		meshes[0]->SetTexture(blueArrow);
		isCurrentlyHovered = true;
	}
	else if (!hovered && isCurrentlyHovered)
	{
		meshes[0]->SetTexture(grayArrow);
		isCurrentlyHovered = false;
	}
}
