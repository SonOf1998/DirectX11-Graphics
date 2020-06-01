#pragma once

#include <memory>

#include "CBufferDataType.h"
#include "Geometry.h"
#include "Texture.h"

class Mesh
{
	std::unique_ptr<Geometry> geometry	= nullptr;
	std::unique_ptr<Material> material	= nullptr;
	std::unique_ptr<Texture> texture	= nullptr;

public:

	Mesh(Geometry*, Material* = nullptr);
	~Mesh() = default;

	void SetTexture(Texture*);
	Texture* GetTexture() const;
	Geometry* GetGeometry() const;
	Material* GetMaterial() const;
};