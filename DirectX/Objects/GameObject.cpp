#include "pch.h"
#include "GameObject.h"

#include "AxisAlignedBoundingBox.h"
#include "Camera.h"
#include "Mesh.h"

GameObject::GameObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMVECTOR position, XMVECTOR scale, XMVECTOR rotationAxis, float angle) : position(position), scale(scale)
{
	rotation = XMMatrixRotationAxis(rotationAxis, angle);
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(scale);
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(position);
	modelMatrix = scaleMatrix * rotation * translationMatrix;	
}

GameObject::~GameObject() = default;

void GameObject::AddMeshAxisAlignedBoundingBox()
{
	size_t lastIndex = meshes.size() - 1;

	Geometry* geometry = meshes[lastIndex]->GetGeometry();
	
	aabbs.emplace_back(std::make_unique<AxisAlignedBoundingBox>(geometry->xMin, geometry->xMax, geometry->yMin, geometry->yMax, geometry->zMin, geometry->zMax));
	aabbs[lastIndex]->RecalculateVertices(modelMatrix);
}

XMMATRIX GameObject::GetModelMatrix() const
{
	return modelMatrix;
}

void GameObject::SetPosition(const XMVECTOR& position)
{
	this->position = position;
}

XMVECTOR GameObject::GetPosition() const
{
	return position;
}

void GameObject::AddMesh(Mesh* mesh)
{
	meshes.emplace_back(mesh);
	AddMeshAxisAlignedBoundingBox();
}

void GameObject::AddMesh(std::unique_ptr<Mesh>&& newMesh)
{
	meshes.push_back(std::move(newMesh));
	AddMeshAxisAlignedBoundingBox();
}

void GameObject::CopyAndAddMesh(Mesh mesh)
{
	meshes.push_back(std::make_unique<Mesh>(mesh));
	AddMeshAxisAlignedBoundingBox();
}

void GameObject::SetHidden(bool hidden) noexcept
{
	isHidden = hidden;
}

