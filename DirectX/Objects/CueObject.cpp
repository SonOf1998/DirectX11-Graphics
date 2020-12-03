#include "pch.h"
#include "CueObject.h"

// from DirectX
#include "AssimpMultiModel.h"
#include "Camera.h"
#include "CBufferDataType.h"
#include "QuadGeometry.h"
#include "OverlayObject.h"
#include "PerspectiveCamera.h"
#include "Pipeline.h"
#include "Graphics.h"
#include "Mesh.h"
#include "Light.h"
#include "Resources.h"
#include "WhiteBallObject.h"

// from SnookerLogic
#include "RoundManager.h"

// from Sounds
#include "SoundManager.h"

CueObject::CueObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, PerspectiveCamera* camera, XMVECTOR position, XMVECTOR scale, XMVECTOR rotationAxis, float rotationAngle) :
	GameObject(device, deviceContext, position, scale, rotationAxis, rotationAngle),
	camera(camera)
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

void CueObject::InitWhiteBall(WhiteBallObject* whiteBall)
{
	this->whiteBall = whiteBall;
}

void CueObject::Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera /* nullptr */, Light* light/* nullptr */)
{
	RoundManager& rm = RoundManager::GetInstance();
	if (!rm.IsWhitePlaced() || rm.IsRoundGoing() || rm.IsInWalkMode())
		return;


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

	pipeline->SetSampler(FILTERING::BILINEAR, 0);
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
	RoundManager& rm = RoundManager::GetInstance();
	if (!rm.IsWhitePlaced() || rm.IsRoundGoing() || rm.IsInWalkMode())
		return;
	
	/*DirLightVP dirLightVP;
	dirLightVP.GetData().viewProj = Transpose(light->GetViewProjMatrix());

	M m;
	m.GetData().model = Transpose(modelMatrix);
	pipeline->SetCBuffer(&dirLightVP, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);
	pipeline->SetCBuffer(&m, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);

	pipeline->Use();
	for (auto& mesh : meshes)
	{
		mesh->GetGeometry()->Draw(deviceContext);
	}*/
	// works but ugly visuals!
}

struct Thrust
{
	long dy;
	float dt;
};

#define MAX_CACHE_SIZE 3
std::deque<Thrust> thrustCache;

void CueObject::Animate(float t, float dt)
{
	XMVECTOR whitePos = camera->GetWhiteBallPos();
	XMVECTOR cameraPos = camera->GetPosition();
	XMFLOAT3 cameraPosF = camera->GetPositionF();
	XMFLOAT3 whitePosF;
	XMStoreFloat3(&whitePosF, whitePos);

	XMVECTOR dir2D = XMVectorSet(whitePosF.x - cameraPosF.x, 0, whitePosF.z - cameraPosF.z, 0);
	dir2D = XMVector3Normalize(dir2D);
	XMFLOAT3 dir2DF;
	XMStoreFloat3(&dir2DF, dir2D);
	XMFLOAT3 dir2DFNormal = XMFLOAT3(-dir2DF.z, 0, dir2DF.x);

	float angle = acosf(XMVectorGetX(XMVector3Dot(dir2D, orientation)));
	//Logger::print(std::to_string(XMVectorGetX(XMVector3Dot(dir2D, orientation))));
	//Logger::print(std::to_string(angle));

	// as dot returns the smallest angle
	// we need to check whether the user started aiming at the right direction
	if (dir2DF.x < 0)
		angle *= -1;

	RoundManager& rm = RoundManager::GetInstance();
	if (!rm.IsRoundGoing() && WhiteBallObject::isInShootMode && InputClass::LeftMBDown())
	{
		POINT curMove = InputClass::GetCursorMove();
		long dy = curMove.y;

		cdf += dy * 0.003f;

		if (thrustCache.size() == MAX_CACHE_SIZE)
		{
			thrustCache.pop_front();
		}
		thrustCache.push_back(Thrust {dy, dt});

		cdf = std::clamp(cdf, CDF_LOWER_BOUND, CDF_UPPER_BOUND);
		
		//todo fwd list for proper speed factor

		long sumDy = 0;
		float sumDt = 0;
		for (uint i = 0; i < thrustCache.size(); ++i)
		{
			auto& cacheElement = thrustCache[i];
			sumDy += cacheElement.dy;
			sumDt += cacheElement.dt;
		}

		float speedFactor = -sumDy / (750 * sumDt);
		if (cdf == CDF_LOWER_BOUND)
		{
			SoundManager& sm = SoundManager::GetInstance();
			XMFLOAT3 soundEffectCenter;
			XMStoreFloat3(&soundEffectCenter, whiteBall->GetPosition());
			sm.PlaySound(sm.GetCueBallCollisionSoundFileName(speedFactor),
				soundEffectCenter,
				nullptr,
				nullptr
			);


			whiteBall->InitiateShot(this, speedFactor, dir2D);
		}
	}
	
	if (!rm.IsRoundGoing() && WhiteBallObject::isInSpinMode && InputClass::LeftMBDown()) {
		POINT curMove = InputClass::GetCursorMove();
		float dx = curMove.x;
		float dy = curMove.y;

		if ((SpinOverlayObject::dx + dx) * (SpinOverlayObject::dx + dx) + (SpinOverlayObject::dy - dy) * (SpinOverlayObject::dy - dy) <= 160 * 160)
		{
			SpinOverlayObject::dx += dx;
			SpinOverlayObject::dy -= dy;
		}			
	}

	float spinx = SpinOverlayObject::dx / 2.0f / Graphics::screenWidth;
	float spiny = SpinOverlayObject::dy / 3.0f / Graphics::screenHeight;

	position = XMVectorSet(whitePosF.x - cdf * dir2DF.x + dir2DFNormal.x * spinx, SNOOKER_TABLE_POS_Y + BALL_RADIUS + spiny, whitePosF.z - cdf * dir2DF.z + dir2DFNormal.z * spinx, 0);
	//Logger::print(std::to_string(cameraPosF.x - dir2DF.x));

	modelMatrix = XMMatrixScalingFromVector(scale) * XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), -angle) * XMMatrixTranslationFromVector(position);
}

void CueObject::ResetCDF() noexcept
{
	cdf = CDF_DEFAULT_VAL;
}

 