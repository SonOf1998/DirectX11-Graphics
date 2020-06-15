#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(const std::shared_ptr<Geometry>& geometry, const std::shared_ptr<Material>& material)
{
	this->geometry = geometry;
	this->material = material;
}

Mesh::Mesh(const Mesh& mesh)  
{
	geometry = mesh.geometry;
	material = mesh.material;
	texture = mesh.texture;
}

Mesh& Mesh::operator=(Mesh mesh)
{
	std::swap(geometry, mesh.geometry);
	std::swap(material, mesh.material);
	std::swap(texture, mesh.texture);

	return *this;
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
