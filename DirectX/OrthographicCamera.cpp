#include "pch.h"
#include "OrthographicCamera.h"

#include "Graphics.h"

OrthographicCamera::OrthographicCamera() : Camera()
{
	viewProjMatrix = XMMatrixOrthographicRH(static_cast<float>(Graphics::screenWidth), static_cast<float>(Graphics::screenHeight), 0, 1);
	//viewProjMatrixInv = Inverse(viewProjMatrix);
}

void OrthographicCamera::MarkForUpdate() noexcept
{
	needsUpdate = true;
}

void OrthographicCamera::Animate(float /*t*/, float /*dt*/)
{
	if (needsUpdate)
	{
		viewProjMatrix = XMMatrixOrthographicRH(static_cast<float>(Graphics::screenWidth), static_cast<float>(Graphics::screenHeight), 0, 1);
		//viewProjMatrixInv = Inverse(viewProjMatrix);
		needsUpdate = true;
	}	
}
