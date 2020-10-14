#include "pch.h"
#include "OverlayObject.h"

// from DirectX
#include "AssimpModel.h"
#include "Camera.h"
#include "Graphics.h"
#include "Pipeline.h"
#include "Resources.h"

// from SnookerLogic
#include "RoundManager.h"

static LPCWSTR GetProperTextureForBall(BALL ball)
{
	switch (ball)
	{
	case RED_BALL:
		return RED_BALL_OVERLAY_TEXTURE;
	case YELLOW_BALL:
		return YELLOW_BALL_OVERLAY_TEXTURE;
	case GREEN_BALL:
		return GREEN_BALL_OVERLAY_TEXTURE;
	case BROWN_BALL:
		return BROWN_BALL_OVERLAY_TEXTURE;
	case BLUE_BALL:
		return BLUE_BALL_OVERLAY_TEXTURE;
	case PINK_BALL:
		return PINK_BALL_OVERLAY_TEXTURE;
	case BLACK_BALL:
		return BLACK_BALL_OVERLAY_TEXTURE;
	case WHITE_BALL:
		return WHITE_BALL_OVERLAY_TEXTURE;
	case NOMINATE_WARNING:
		return NOMINATE_OVERLAY_TEXTURE;
	default:
		throw std::runtime_error("Switch is not exhaustive in OverlayObject.cpp!");
	}
}

#define SIZE 25
XMVECTOR GetPositionFromSpot(OVERLAY_SPOT spot)
{
	float x = static_cast<float>(Graphics::screenWidth);
	float y = static_cast<float>(Graphics::screenHeight);

	XMVECTOR pos;

	if (spot == OVERLAY_TARGET)
	{
		pos = 2 * XMVectorSet(87 / x, 1 - (17 / y), 0, 0) + XMVectorSet(-1, -1, 0, 0);
	}
	else if (spot == OVERLAY_SPIN)
	{
		// todo
	}
	else
	{
		// 0->tg
		// 1->spin
		// 2->pot1
		// ...
		// 7->pot6
		int spotId = spot;
		int offsetFromXEnd = (7 - spotId) * (SIZE + 5) + (SIZE / 2) + 5;
		
		pos = 2 * XMVectorSet(1 - offsetFromXEnd / x, 1 - (17 / y), 0, 0) + XMVectorSet(-1, -1, 0, 0);
	}

	return pos;
}

float GetSizeFromSpot(OVERLAY_SPOT spot)
{
	float size;

	if (spot == OVERLAY_SPIN)
	{
		// todo
	}
	else
	{
		size = SIZE;
	}

	return size;
}


OverlayObject::OverlayObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float ttl, BALL ball, OVERLAY_SPOT overlaySpot) : 
	GameObject(),
	ttl(ttl),
	ball(ball),
	overlaySpot(overlaySpot)
{
	float x = static_cast<float>(Graphics::screenWidth);
	float y = static_cast<float>(Graphics::screenHeight);
	float size = GetSizeFromSpot(overlaySpot);
	scale = XMVectorSet(size / x, size / y, 1, 1);
	position = GetPositionFromSpot(overlaySpot);
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(scale);
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(position);
	modelMatrix = scaleMatrix * translationMatrix;
	

	std::shared_ptr<Geometry> overlayGeometry = std::make_shared<AssimpModel<PT>>(device, QUAD_MODEL);
	std::shared_ptr<Texture>  overlayTexture = std::make_shared<Texture>(device, deviceContext, GetProperTextureForBall(ball), 0);
	std::unique_ptr<Mesh> overlayMesh = std::make_unique<Mesh>(overlayGeometry);
	overlayMesh->SetTexture(overlayTexture);	
	AddMesh(std::move(overlayMesh));
}

bool OverlayObject::StillLiving() const noexcept
{
	return ttl > 0;
}

void OverlayObject::Kill() noexcept
{
	// subtracting 5 sec is more than enough
	ttl -= 5;
}

void OverlayObject::MarkForUpdate() noexcept
{
	needsUpdate = true;
}

void OverlayObject::Render(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Camera* camera, Light*)
{
	if (!StillLiving())
		return;

	XMMATRIX viewProj = XMMatrixIdentity();
	//XMMATRIX viewProjInv = XMMatrixIdentity();
	if (camera != nullptr)
	{
		//viewProj = camera->GetViewProjMatrix();
		//viewProjInv = camera->GetViewProjMatrixInv();
	}


	MVP mvp;
	mvp.GetData().model = Transpose(modelMatrix);
	mvp.GetData().viewProj = Transpose(viewProj);

	pipeline->SetSampler(FILTERING::TRILINEAR, 0);

	pipeline->SetCBuffer(&mvp, CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER);

	for (auto& mesh : meshes)
	{
		pipeline->SetTexture(mesh->GetTexture());
		mesh->GetGeometry()->Draw(deviceContext, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

}

void OverlayObject::Animate(float t, float dt)
{
	if (needsUpdate)
	{
		float x = static_cast<float>(Graphics::screenWidth);
		float y = static_cast<float>(Graphics::screenHeight);
		float size = GetSizeFromSpot(overlaySpot);
		scale = XMVectorSet(size / x, size / y, 1, 1);
		position = GetPositionFromSpot(overlaySpot);
		XMMATRIX scaleMatrix = XMMatrixScalingFromVector(scale);
		XMMATRIX translationMatrix = XMMatrixTranslationFromVector(position);
		modelMatrix = scaleMatrix * translationMatrix;

		needsUpdate = false;
	}	

	RoundManager& rm = RoundManager::GetInstance();
	if (!rm.IsRoundGoing())
		ttl -= dt;

}
