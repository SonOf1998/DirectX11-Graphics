#pragma once
#include "Renderable.h"

class AxisAlignedBoundingBox;
class Mesh;

class ArmchairSet : public Renderable
{
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<std::unique_ptr<AxisAlignedBoundingBox>> aabbs;

	XMMATRIX modelMatrix1; // playerA's chair
	XMMATRIX invModelMatrix1;
	XMMATRIX modelMatrix2; // playerB's chair
	XMMATRIX invModelMatrix2;

	// As these are fixed position objects, we only initialize these
	// matrixes in the constructor and will not update them later on

public:

	ArmchairSet(ID3D11Device*, ID3D11DeviceContext*);
	~ArmchairSet();

	virtual void Render(ID3D11DeviceContext* device, Pipeline* pipeline, Camera* camera = nullptr, Light* light = nullptr) override;
	virtual void RenderToShadowMap(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Light* light);
};

