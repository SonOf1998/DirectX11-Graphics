#include "pch.h"
#include "SnookerTableObject.h"

#include "AssimpMultiModel.h"
#include "AxisAlignedBoundingBox.h"
#include "Camera.h"
#include "Light.h"
#include "Pipeline.h"
#include "Resources.h"

void SnookerTableObject::BuildTable(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	auto geometries = AssimpMultiModel<PNT>(device, SNOOKER_TABLE_MODEL).GetGeometries();

	std::shared_ptr<Texture> frameTexture			= std::make_shared<Texture>(device, deviceContext, TABLE_FRAME_TEXTURE,			0);
	std::shared_ptr<Texture> tableTexture			= std::make_shared<Texture>(device, deviceContext, TABLE_CLOTH_TEXTURE,			0);
	std::shared_ptr<Texture> xingPaiStarLogo		= std::make_shared<Texture>(device, deviceContext, TABLE_XINGPAISTAR_TEXTURE,	0);
	std::shared_ptr<Texture> sideWallTexture		= std::make_shared<Texture>(device, deviceContext, TABLE_CUSHION_TEXTURE,		0);
	std::shared_ptr<Texture> pocketDimmerTexture	= std::make_shared<Texture>(device, deviceContext, TABLE_POCKET_TEXTURE,		0);
	
	std::shared_ptr<Geometry> frameGeometry         = std::move(geometries[0]);
	std::shared_ptr<Geometry> pocketDimmerGeometry  = std::move(geometries[5]);
	std::shared_ptr<Geometry> sideWallGeometry		= std::move(geometries[4]);
	std::shared_ptr<Geometry> tableGeometry			= std::move(geometries[3]);
	std::shared_ptr<Geometry> xingPaiStarGeomRear	= std::move(geometries[1]);
	std::shared_ptr<Geometry> xingPaiStarGeomFront	= std::move(geometries[2]);

	std::shared_ptr<Material> dullMaterial = std::make_shared<Material>(XMFLOAT3(0.3f,0.3f, 0.3f), XMFLOAT3(0, 0, 0), 1.0f);

	std::unique_ptr<Mesh> frameMesh = std::make_unique<Mesh>(frameGeometry, dullMaterial);
	frameMesh->SetTexture(frameTexture);
	std::unique_ptr<Mesh> sideWallMesh = std::make_unique<Mesh>(sideWallGeometry, dullMaterial);
	sideWallMesh->SetTexture(sideWallTexture);
	std::unique_ptr<Mesh> pocketDimmerMesh = std::make_unique<Mesh>(pocketDimmerGeometry, dullMaterial);
	pocketDimmerMesh->SetTexture(pocketDimmerTexture);
	std::unique_ptr<Mesh> tableMesh = std::make_unique<Mesh>(tableGeometry, dullMaterial);
	tableMesh->SetTexture(tableTexture);
	std::unique_ptr<Mesh> xinPaiStarRearMesh = std::make_unique<Mesh>(xingPaiStarGeomRear, dullMaterial);
	xinPaiStarRearMesh->SetTexture(xingPaiStarLogo);
	std::unique_ptr<Mesh> xinPaiStarFrontMesh = std::make_unique<Mesh>(xingPaiStarGeomFront, dullMaterial);
	xinPaiStarFrontMesh->SetTexture(xingPaiStarLogo);

	AddMesh(std::move(frameMesh));
	AddMesh(std::move(sideWallMesh));
	AddMesh(std::move(pocketDimmerMesh));
	AddMesh(std::move(tableMesh));
	AddMesh(std::move(xinPaiStarRearMesh));
	AddMesh(std::move(xinPaiStarFrontMesh));
}

SnookerTableObject::SnookerTableObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMVECTOR position, XMVECTOR scale, XMVECTOR rotationAxis, float angle) : GameObject(device, deviceContext, position, scale, rotationAxis, angle)
{
	BuildTable(device, deviceContext);
}

void SnookerTableObject::Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera /* nullptr */, Light* light /* = nullptr */)
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

	MVPMInv mpvminv;
	mpvminv.GetData().model = Transpose(modelMatrix);
	mpvminv.GetData().viewProj = Transpose(viewProj);
	mpvminv.GetData().modelInv = Inverse(modelMatrix);

	pipeline->SetSampler(FILTERING::ANISOTROPIC_X4, 0);
	pipeline->SetCBuffer(&mpvminv, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);


	CameraCBuffer cameraCBuffer;
	cameraCBuffer.GetData().position = camera->GetPosition();
	pipeline->SetCBuffer(&cameraCBuffer, CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER);

	for (uint i = 0; i < meshes.size(); ++i)
	{		
		// aabb recalculation happens only in base class' contructor
		// as the table itself is not update dynamically
		if (!aabbs[i]->IsInsideViewFrustum(viewProj, viewProjInv))
		{
			// for easier debugging
			continue;
		}
		else
		{
			std::unique_ptr<Mesh>& mesh = meshes[i];
			pipeline->SetTexture(mesh->GetTexture());
			pipeline->SetCBuffer(mesh->GetMaterial(), CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER);
			mesh->GetGeometry()->Draw(deviceContext);
		}
	}
}

void SnookerTableObject::RenderToShadowMap(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Light* light)
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

void SnookerTableObject::Animate(float t, float dt)
{
}
