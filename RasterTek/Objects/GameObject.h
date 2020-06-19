#pragma once

#include "Mesh.h"
#include "Renderable.h"

class Camera;
class Pipeline;
class Light;

class GameObject : public Renderable
{

protected:

	std::vector<std::unique_ptr<Mesh>> meshes;

	XMMATRIX modelMatrix;

	XMVECTOR position;
	XMVECTOR scale;
	XMMATRIX rotation;
		
public:

	GameObject(ID3D11Device*, ID3D11DeviceContext*, XMVECTOR = XMVectorSet(0,0,0,0), XMVECTOR = XMVectorSet(1,1,1,1), XMVECTOR = XMVectorSet(0,0,1,0), float = 0.0f);
	GameObject(const GameObject&)	= delete;
	virtual ~GameObject()			= default;

	XMMATRIX GetModelMatrix() const;
	void AddMesh(Mesh*);
	void AddMesh(std::unique_ptr<Mesh>&&);
	void CopyAndAddMesh(Mesh);
};
