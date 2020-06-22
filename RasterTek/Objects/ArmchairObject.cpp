#include "pch.h"
#include "ArmchairObject.h"

#include "AssimpMultiModel.h"
#include "Camera.h"
#include "CBufferDataType.h"
#include "Light.h"
#include "Pipeline.h"
#include "Resources.h"

ArmchairObject::ArmchairObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMVECTOR position, XMVECTOR scale, XMVECTOR rotationAxis, float angle) : GameObject(device, deviceContext, position, scale, rotationAxis, angle)
{
	std::shared_ptr<Texture> bumpTexture	= std::make_shared<Texture>(device, deviceContext, ARMCHAIR_BUMP_TEXTURE,   0);
	std::shared_ptr<Texture> frameTexture	= std::make_shared<Texture>(device, deviceContext, ARMCHAIR_FRAME_TEXTURE,  0);
	std::shared_ptr<Texture> pillowTexture	= std::make_shared<Texture>(device, deviceContext, ARMCHAIR_PILLOW_TEXTURE, 0);



	auto armchairGeometries = AssimpMultiModel<PNT>(device, ARMCHAIR_MODEL).GetGeometries();

	std::shared_ptr<Material> dullMaterial		= std::make_shared<Material>(XMFLOAT3(0.3f, 0.3f, 0.3f), XMFLOAT3(0, 0, 0), 1.0f);
	std::shared_ptr<Material> shinyWoodMaterial	= std::make_shared<Material>(XMFLOAT3(0.2f, 0.2f, 0.2f), XMFLOAT3(0.7f, 0.7f, 0.7f), 50.0f);

	std::unique_ptr<Mesh> bumpMesh = std::make_unique<Mesh>(armchairGeometries[0], dullMaterial);
	std::unique_ptr<Mesh> frameMesh = std::make_unique<Mesh>(armchairGeometries[2], shinyWoodMaterial);
	std::unique_ptr<Mesh> pillowMesh = std::make_unique<Mesh>(armchairGeometries[1], dullMaterial);
	
	bumpMesh->SetTexture(bumpTexture);
	frameMesh->SetTexture(frameTexture);
	pillowMesh->SetTexture(pillowTexture);

	meshes.push_back(std::move(bumpMesh));
	meshes.push_back(std::move(frameMesh));
	meshes.push_back(std::move(pillowMesh));
}

void ArmchairObject::Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera /* = nullptr */, Light* light /* = nullptr */)
{
	XMMATRIX viewProj = XMMatrixIdentity();
	if (camera != nullptr)
	{
		viewProj = camera->GetViewProjMatrix();
	}

	if (light != nullptr)
	{
		DirLightVP dirLightVP;
		dirLightVP.GetData().viewProj = Transpose(light->GetViewProjMatrix());
		pipeline->SetCBuffer(&dirLightVP, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);
	}

	MMInv mminv;
	mminv.GetData().model = Transpose(modelMatrix);
	mminv.GetData().modelInv = Inverse(modelMatrix);

	pipeline->SetSampler(FILTERING::TRILINEAR, 0);
	pipeline->SetCBuffer(&mminv, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);


	CameraCBuffer cameraCBuffer;	
	cameraCBuffer.GetData().position = camera->GetPosition();
	pipeline->SetCBuffer(&cameraCBuffer, CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER);

	VP vp;
	vp.GetData().viewProj = Transpose(viewProj);
	pipeline->SetCBuffer(&vp, CBUFFER_LOCATION::DOMAIN_SHADER_CBUFFER);

	for (auto& mesh : meshes)
	{
		pipeline->SetTexture(mesh->GetTexture());
		pipeline->SetCBuffer(mesh->GetMaterial(), CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER);
		mesh->GetGeometry()->Draw(deviceContext, D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	}
}

void ArmchairObject::RenderToShadowMap(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Light* light)
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

void ArmchairObject::Animate(float t, float dt)
{
}

