#pragma once

#include "Mesh.h"
#include "Renderable.h"

class AxisAlignedBoundingBox;
class Camera;
class Pipeline;
class Light;

class GameObject : public Renderable
{

protected:

	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<std::unique_ptr<AxisAlignedBoundingBox>> aabbs;

	XMMATRIX modelMatrix;

	XMVECTOR position;
	XMVECTOR scale;
	XMMATRIX rotation;

	bool isHidden = false;
		
	void AddMeshAxisAlignedBoundingBox();

public:

	GameObject(ID3D11Device*, ID3D11DeviceContext*, XMVECTOR = XMVectorSet(0,0,0,0), XMVECTOR = XMVectorSet(1,1,1,1), XMVECTOR = XMVectorSet(0,0,1,0), float = 0.0f);
	GameObject(const GameObject&)	= delete;
	virtual ~GameObject();

	XMMATRIX GetModelMatrix() const;
	void SetPosition(const XMVECTOR&);
	XMVECTOR GetPosition() const;

	void AddMesh(Mesh*);
	void AddMesh(std::unique_ptr<Mesh>&&);
	void CopyAndAddMesh(Mesh);

	void SetHidden(bool hidden) noexcept;
};
