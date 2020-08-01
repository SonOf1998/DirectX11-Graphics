#pragma once

#include "Animatable.h"

// 3D perspective camera
class Camera : public Animatable
{
protected:

	XMVECTOR position;
	XMMATRIX viewProjMatrix;
	XMMATRIX viewProjMatrixInv;

	Camera(const XMVECTOR& position);

public:
		
	XMVECTOR GetPosition()		 const noexcept;
	XMMATRIX GetViewProjMatrix() const noexcept;
	XMMATRIX GetViewProjMatrixInv() const noexcept;
};