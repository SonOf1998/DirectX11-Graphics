#include "pch.h"
#include "ArmchairSet.h"

#include "AssimpMultiModel.h"
#include "AxisAlignedBoundingBox.h"
#include "Camera.h"
#include "CBufferDataType.h"
#include "Light.h"
#include "Mesh.h"
#include "Pipeline.h"
#include "Positions.h"
#include "Resources.h"

ArmchairSet::ArmchairSet(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	XMMATRIX rotation = XMMatrixRotationAxis(ARMCHAIR_ROT_AXIS, ARMCHAIR_ROT_ANGLE);
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(ARMCHAIR_SCALE);

	XMMATRIX translationMatrix1 = XMMatrixTranslationFromVector(ARMCHAIR_1_POS);
	modelMatrix1 = scaleMatrix * rotation * translationMatrix1;

	XMMATRIX translationMatrix2 = XMMatrixTranslationFromVector(ARMCHAIR_2_POS);
	modelMatrix2 = scaleMatrix * rotation * translationMatrix2;

	std::shared_ptr<Texture> bumpTexture = std::make_shared<Texture>(device, deviceContext, ARMCHAIR_BUMP_TEXTURE, 0);
	std::shared_ptr<Texture> frameTexture = std::make_shared<Texture>(device, deviceContext, ARMCHAIR_FRAME_TEXTURE, 0);
	std::shared_ptr<Texture> pillowTexture = std::make_shared<Texture>(device, deviceContext, ARMCHAIR_PILLOW_TEXTURE, 0);

	auto armchairGeometries = AssimpMultiModel<PNT>(device, ARMCHAIR_MODEL).GetGeometries();

	std::shared_ptr<Material> dullMaterial = std::make_shared<Material>(XMFLOAT3(0.3f, 0.3f, 0.3f), XMFLOAT3(0, 0, 0), 1.0f);
	std::shared_ptr<Material> shinyWoodMaterial = std::make_shared<Material>(XMFLOAT3(0.2f, 0.2f, 0.2f), XMFLOAT3(0.7f, 0.7f, 0.7f), 50.0f);

	std::unique_ptr<Mesh> bumpMesh = std::make_unique<Mesh>(armchairGeometries[0], dullMaterial);
	std::unique_ptr<Mesh> pillowMesh = std::make_unique<Mesh>(armchairGeometries[1], dullMaterial);
	std::unique_ptr<Mesh> frameMesh = std::make_unique<Mesh>(armchairGeometries[2], shinyWoodMaterial);

	bumpMesh->SetTexture(bumpTexture);
	frameMesh->SetTexture(frameTexture);
	pillowMesh->SetTexture(pillowTexture);

	meshes.push_back(std::move(bumpMesh));
	meshes.push_back(std::move(pillowMesh));
	meshes.push_back(std::move(frameMesh));

	aabbs.emplace_back(std::make_unique<AxisAlignedBoundingBox>(armchairGeometries[0].get()));
	aabbs.emplace_back(std::make_unique<AxisAlignedBoundingBox>(armchairGeometries[1].get()));
	aabbs.emplace_back(std::make_unique<AxisAlignedBoundingBox>(armchairGeometries[2].get()));
	aabbs.emplace_back(std::make_unique<AxisAlignedBoundingBox>(armchairGeometries[0].get()));
	aabbs.emplace_back(std::make_unique<AxisAlignedBoundingBox>(armchairGeometries[1].get()));
	aabbs.emplace_back(std::make_unique<AxisAlignedBoundingBox>(armchairGeometries[2].get()));

	for (int i = 0; i < meshes.size(); ++i)
	{
		aabbs[i]->RecalculateVertices(modelMatrix1);
	}
	for (int i = meshes.size(); i < 2 * meshes.size(); ++i)
	{
		aabbs[i]->RecalculateVertices(modelMatrix2);
	}
}


ArmchairSet::~ArmchairSet() = default;

void ArmchairSet::Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera /*= nullptr*/, Light* light /*= nullptr*/)
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


	// TODO instanced frustum culling

	for (uint i = 0; i < meshes.size(); ++i)
	{
		std::unique_ptr<Mesh>& mesh = meshes[i];
		uint instanceCount = 2;

		std::vector<XMMATRIX> instanceDataVector;

		if (!aabbs[i]->IsInsideViewFrustum(viewProj, viewProjInv))
		{
			instanceCount--;
		}
		else
		{
			instanceDataVector.push_back(modelMatrix1);
			instanceDataVector.push_back(Inverse(modelMatrix1));
		}
		if (!aabbs[i + meshes.size()]->IsInsideViewFrustum(viewProj, viewProjInv))
		{
			instanceCount--;
		}
		else
		{
			instanceDataVector.push_back(modelMatrix2);
			instanceDataVector.push_back(Inverse(modelMatrix2));
		}

		if (instanceCount == 0)
		{
			continue;
		}

		MMInv<20> mminv;
		for (uint j = 0; j < instanceDataVector.size() / 2; ++j)
		{
			mminv.GetData().model[j] = Transpose(instanceDataVector[2 * j]);
			mminv.GetData().modelInv[j] = instanceDataVector[2 * j + 1];
		}
		
		pipeline->SetCBuffer(&mminv, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);
		pipeline->SetTexture(mesh->GetTexture());
		pipeline->SetCBuffer(mesh->GetMaterial(), CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER);
		mesh->GetGeometry()->DrawInstanced(deviceContext, instanceCount, D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	}

	//for (auto& mesh : meshes)
	//{
	//	pipeline->SetTexture(mesh->GetTexture());
	//	pipeline->SetCBuffer(mesh->GetMaterial(), CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER);
	//	mesh->GetGeometry()->DrawInstanced(deviceContext, 2, D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	//}
}

void ArmchairSet::RenderToShadowMap(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Light* light)
{
	// todo
}
