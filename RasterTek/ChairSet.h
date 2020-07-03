#pragma once
#include "Renderable.h"

class AxisAlignedBoundingBox;
class Mesh;

class ChairSet : public Renderable
{
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<std::unique_ptr<AxisAlignedBoundingBox>> aabbs;

	std::vector<XMMATRIX> modelMatrices;
	std::vector<XMMATRIX> invModelMatrices;

public:

	ChairSet(ID3D11Device*, ID3D11DeviceContext*);
	~ChairSet();

	virtual void Render(ID3D11DeviceContext* device, Pipeline* pipeline, Camera* camera = nullptr, Light* light = nullptr) override;
	virtual void RenderToShadowMap(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Light* light);
};

