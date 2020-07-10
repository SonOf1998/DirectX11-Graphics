#include "pch.h"
#include "ChairSet.h"

#include "AssimpMultiModel.h"
#include "AxisAlignedBoundingBox.h"
#include "Camera.h"
#include "CBufferDataType.h"
#include "Light.h"
#include "Mesh.h"
#include "Pipeline.h"
#include "Positions.h"
#include "Resources.h"

ChairSet::ChairSet(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(CHAIR_SCALE);

	for (int i = 0; i < 4; ++i)	// rows
	{
		for (int j = 0; j < 6 + i; ++j)
		{
			float angleStep = (toRad(60.0f + i * 5.0f)) / (6 + i);
			float distance	= (10.0f + i * 2);
			float angle0    = (toRad(115.0f + i * 2.5f));
			float angle     = angle0 - j * angleStep;

			XMVECTOR translation = XMVectorSet(distance * cosf(angle), i * CHAIR_ELEVATION, -distance * sinf(angle), 1);

			XMMATRIX rotationMatrix = XMMatrixRotationAxis(Y_AXIS, angle - toRad(90));
			XMMATRIX translationMatrix = XMMatrixTranslationFromVector(translation);

			XMMATRIX modelMatrix = scaleMatrix * rotationMatrix * translationMatrix;
			modelMatrices.push_back(modelMatrix);
			invModelMatrices.push_back(XMMatrixInverse(nullptr, modelMatrix));
		}
	}

	std::shared_ptr<Texture> fabricTexture = std::make_shared<Texture>(device, deviceContext, CHAIR_FABRIC_TEXTURE, 0);
	std::shared_ptr<Texture> frameTexture = std::make_shared<Texture>(device, deviceContext, CHAIR_FRAME_TEXTURE, 0);

	auto chairGeometries = AssimpMultiModel<PNT>(device, CHAIR_MODEL).GetGeometries();

	std::shared_ptr<Material> dullMaterial = std::make_shared<Material>(XMFLOAT3(0.3f, 0.3f, 0.3f), XMFLOAT3(0, 0, 0), 1.0f);
	std::shared_ptr<Material> lightWoodMaterial = std::make_shared<Material>(XMFLOAT3(0.2f, 0.2f, 0.2f), XMFLOAT3(0.3f, 0.3f, 0.3f), 50.0f);

	std::unique_ptr<Mesh> backrestMesh = std::make_unique<Mesh>(chairGeometries[0], dullMaterial);
	std::unique_ptr<Mesh> legMesh = std::make_unique<Mesh>(chairGeometries[1], lightWoodMaterial);
	std::unique_ptr<Mesh> pillowMesh = std::make_unique<Mesh>(chairGeometries[2], dullMaterial);

	backrestMesh->SetTexture(fabricTexture);
	legMesh->SetTexture(frameTexture);
	pillowMesh->SetTexture(fabricTexture);

	meshes.push_back(std::move(backrestMesh));
	meshes.push_back(std::move(legMesh));
	meshes.push_back(std::move(pillowMesh));


	for (int i = 0; i < 6 + 7 + 8 + 9; ++i)
	{
		for (uint j = 0; j < meshes.size(); ++j)
		{
			std::unique_ptr<AxisAlignedBoundingBox> aabb = std::make_unique<AxisAlignedBoundingBox>(chairGeometries[j].get());
			aabb->RecalculateVertices(modelMatrices[i]);
			aabbs.emplace_back(std::move(aabb));			
		}
	}
}

ChairSet::~ChairSet() = default;

void ChairSet::Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera /*= nullptr*/, Light* light /*= nullptr*/)
{
	XMMATRIX viewProj = XMMatrixIdentity();
	XMMATRIX viewProjInv = XMMatrixIdentity();
	if (camera != nullptr)
	{
		viewProj = camera->GetViewProjMatrix();
		viewProjInv = camera->GetViewProjMatrixInv();
	}

	if (light != nullptr)
	{
		DirLightVP dirLightVP;
		dirLightVP.GetData().viewProj = Transpose(light->GetViewProjMatrix());
		pipeline->SetCBuffer(&dirLightVP, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);
	}

	pipeline->SetSampler(FILTERING::TRILINEAR, 0);

	CameraCBuffer cameraCBuffer;
	cameraCBuffer.GetData().position = camera->GetPosition();
	pipeline->SetCBuffer(&cameraCBuffer, CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER);

	VP vp;
	vp.GetData().viewProj = Transpose(viewProj);
	pipeline->SetCBuffer(&vp, CBUFFER_LOCATION::DOMAIN_SHADER_CBUFFER);

	for (uint i = 0; i < meshes.size(); ++i)
	{
		std::unique_ptr<Mesh>& mesh = meshes[i];
		size_t instanceCount = modelMatrices.size();

		std::vector<XMMATRIX> instanceDataVector;

		for (uint j = 0; j < modelMatrices.size(); ++j)
		{
			if (aabbs[j * meshes.size() + i]->IsInsideViewFrustum(viewProj, viewProjInv))
			{
				instanceDataVector.push_back(modelMatrices[j]);
				instanceDataVector.push_back(invModelMatrices[j]);
			}
			else
			{
				instanceCount--;
			}
		}

		MMInv<MaxInstanceCount> mminv;
		for (uint j = 0; j < instanceDataVector.size() / 2; ++j)
		{
			mminv.GetData().model[j] = Transpose(instanceDataVector[2 * j]);
			mminv.GetData().modelInv[j] = instanceDataVector[2 * j + 1];
		}

		pipeline->SetCBuffer(&mminv, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);
		pipeline->SetTexture(mesh->GetTexture());
		pipeline->SetCBuffer(mesh->GetMaterial(), CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER);
		mesh->GetGeometry()->DrawInstanced(deviceContext, static_cast<UINT>(instanceCount), D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	}
}


void ChairSet::RenderToShadowMap(ID3D11DeviceContext * deviceContext, Pipeline * pipeline, Light * light)
{
	// TODO
}
