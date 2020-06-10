#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(const std::shared_ptr<Geometry>& geometry, const std::shared_ptr<Material>& material)
{
	this->geometry = geometry;
	this->material = material;
}

void Mesh::SetTexture(const std::shared_ptr<Texture>& texture)
{
	this->texture = texture;
}

Texture* Mesh::GetTexture() const
{
	return texture.get();
}

Geometry* Mesh::GetGeometry() const
{
	return geometry.get();
}

Material* Mesh::GetMaterial() const
{
	return material.get();
}
