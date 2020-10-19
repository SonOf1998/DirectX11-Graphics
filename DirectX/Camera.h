#pragma once

#include "Animatable.h"

// 3D perspective camera
class Camera : public Animatable
{
protected:

	XMVECTOR position;
	XMMATRIX projection;
	XMMATRIX viewProjMatrix;
	XMMATRIX viewProjMatrixInv;

	XMVECTOR whiteBallPos;
	XMVECTOR targetBallPos;		// if there are more then the closest

	Camera() = default;
	Camera(const XMVECTOR& position);

public:
		
	XMVECTOR GetPosition()		    const noexcept;
	XMMATRIX GetProjMatrix()		const noexcept;
	XMMATRIX GetViewProjMatrix()    const noexcept;
	XMMATRIX GetViewProjMatrixInv() const noexcept;
	void SetWhiteBallPos(const XMVECTOR& v) noexcept;
	void SetTargetBallPos(const XMVECTOR& v) noexcept;
	XMVECTOR GetWhiteBallPos() const;
	XMVECTOR GetTargetBallPos() const;
};
