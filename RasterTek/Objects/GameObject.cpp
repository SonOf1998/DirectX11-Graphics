#include "pch.h"
#include "GameObject.h"

#include "Camera.h"
#include "Mesh.h"

GameObject::GameObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMVECTOR position, XMVECTOR scale, XMVECTOR rotationAxis, float angle) : position(position), scale(scale)
{
	rotation = XMMatrixRotationAxis(rotationAxis, angle);
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(scale);
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(position);
	modelMatrix = scaleMatrix * rotation * translationMatrix;	
}

XMMATRIX GameObject::GetModelMatrix() const
{
	return modelMatrix;
}

void GameObject::AddMesh(Mesh* mesh)
{
	meshes.emplace_back(mesh);
}

void GameObject::AddMesh(std::unique_ptr<Mesh>&& newMesh)
{
	meshes.push_back(std::move(newMesh));
}

void GameObject::CopyAndAddMesh(Mesh mesh)
{
	meshes.push_back(std::make_unique<Mesh>(mesh));
}

