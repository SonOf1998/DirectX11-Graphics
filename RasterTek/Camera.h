#pragma once

#include "CBufferDataType.h"

// 3D perspective camera
class Camera
{
	XMVECTOR position;

	XMVECTOR up		= XMVectorSet(0, 1, 0, 0);
	XMVECTOR right	= XMVectorSet(1, 0, 0, 0);
	XMVECTOR ahead	= XMVectorSet(0, 0,-1, 0);

	float yaw;
	float pitch;
	float roll;

	float fovy;
	float aspectRatio;

	float zNear;
	float zFar;

	XMMATRIX viewProjMatrix;

public:

	// camera parameters given by lookAt; up vector is fixed (0,1,0)
	Camera(const XMVECTOR& position, const XMVECTOR& lookAt, float aspectRatio = 1.0f, float fovy = XM_PI / 3, float zNear = 0.1f, float zFar = 100.0f);
	// camera parameters given by Euler angles, preferred as it is easier to see through
	Camera(const XMVECTOR& position, float aspectRatio = 1.0f, float yaw = 0.0f, float pitch = 0.0f, float roll = 0.0f, float fovy = XM_PI / 3, float zNear = 0.1f, float zFar = 100.0f);
	Camera(const Camera&)	= default;
	~Camera()				= default;

	float GetYaw		()				  const	  noexcept;
	void  SetYaw		(float)			  		  noexcept;
	void  SetYawDeg		(float)			  		  noexcept;
	float GetPitch		()				  const   noexcept;
	void  SetPitch		(float)			  		  noexcept;
	void  SetPitchDeg	(float)			  		  noexcept;
	float GetRoll	    ()				  const   noexcept;
	void  SetRoll		(float)			  		  noexcept;
	void  SetRollDeg	(float)			  		  noexcept;
	XMVECTOR GetPosition()				  const	  noexcept;
	void  SetPosition	(const XMVECTOR&)		  noexcept;
	void  SetLookAt		(const XMVECTOR&);
	void  SetAspectRatio(float newAspectRatio)	  noexcept;
	
	XMMATRIX GetViewProjMatrix() const;
	CameraCBuffer GetCBuffer();

	void Update();
	void Animate(float, float);
};
