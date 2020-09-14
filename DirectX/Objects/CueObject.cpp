#include "pch.h"
#include "CueObject.h"

#include "AssimpMultiModel.h"
#include "Camera.h"
#include "CBufferDataType.h"
#include "QuadGeometry.h"
#include "Pipeline.h"
#include "Mesh.h"
#include "Light.h"
#include "Resources.h"

CueObject::CueObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMVECTOR position, XMVECTOR scale, XMVECTOR rotationAxis, float rotationAngle) :
	GameObject(device, deviceContext, position, scale, rotationAxis, rotationAngle)
{
	auto geometries = AssimpMultiModel<PNT>(device, CUE_MODEL).GetGeometries();

	std::shared_ptr<Texture> leatherTexture = std::make_shared<Texture>(device, deviceContext, CUE_LEATHER_TEXTURE, 0);
	std::shared_ptr<Texture> flangeTexture = std::make_shared<Texture>(device, deviceContext, CUE_FLANGE_TEXTURE, 0);
	std::shared_ptr<Texture> lightwoodTexture = std::make_shared<Texture>(device, deviceContext, CUE_LIGHTWOOD_TEXTURE, 0);
	std::shared_ptr<Texture> darkwoodTexture = std::make_shared<Texture>(device, deviceContext, CUE_DARKWOOD_TEXTURE, 0);

	std::shared_ptr<Geometry> leatherGeometry = std::move(geometries[3]);
	std::shared_ptr<Geometry> flangeGeometry = std::move(geometries[2]);
	std::shared_ptr<Geometry> lightwoodGeometry = std::move(geometries[1]);
	std::shared_ptr<Geometry> darkwoodGeometry = std::move(geometries[0]);

	std::shared_ptr<Material> dullMaterial = std::make_shared<Material>(XMFLOAT3(0.3f, 0.3f, 0.3f), XMFLOAT3(0, 0, 0), 1.0f);

	std::unique_ptr<Mesh> leatherMesh = std::make_unique<Mesh>(leatherGeometry, dullMaterial);
	leatherMesh->SetTexture(leatherTexture);
	std::unique_ptr<Mesh> flangeMesh = std::make_unique<Mesh>(flangeGeometry, dullMaterial);
	flangeMesh->SetTexture(flangeTexture);
	std::unique_ptr<Mesh> lightwoodMesh = std::make_unique<Mesh>(lightwoodGeometry, dullMaterial);
	lightwoodMesh->SetTexture(lightwoodTexture);
	std::unique_ptr<Mesh> darkwoodMesh = std::make_unique<Mesh>(darkwoodGeometry, dullMaterial);
	darkwoodMesh->SetTexture(darkwoodTexture);

	AddMesh(std::move(leatherMesh));
	AddMesh(std::move(flangeMesh));
	AddMesh(std::move(lightwoodMesh));
	AddMesh(std::move(darkwoodMesh));
}

void CueObject::Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera /* nullptr */, Light* light/* nullptr */)
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
		std::unique_ptr<Mesh>& mesh = meshes[i];
		pipeline->SetTexture(mesh->GetTexture());
		pipeline->SetCBuffer(mesh->GetMaterial(), CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER);
		mesh->GetGeometry()->Draw(deviceContext);		
	}
}

void CueObject::RenderToShadowMap(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Light* light)
{

}

void CueObject::Animate(float t, float dt)
{
	
}
