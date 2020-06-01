#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(Geometry* geometry, Material* material)
{
	this->geometry.reset(geometry);
	this->material.reset(material);
}

void Mesh::SetTexture(Texture* texture)
{
	this->texture.reset(texture);
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
