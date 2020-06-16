#pragma once

#include <memory>

#include "CBufferDataType.h"
#include "Geometry.h"
#include "Texture.h"

class Mesh
{
	std::shared_ptr<Geometry> geometry	= nullptr;
	std::shared_ptr<Material> material	= nullptr;
	std::shared_ptr<Texture>  texture	= nullptr;

public:

	Mesh(const std::shared_ptr<Geometry>&, const std::shared_ptr<Material>& = nullptr);
	Mesh(const Mesh&);
	Mesh& operator=(Mesh);
	~Mesh() = default;


	void SetTexture(const std::shared_ptr<Texture>&);

	Texture* GetTexture() const;
	Geometry* GetGeometry() const;
	Material* GetMaterial() const;
};