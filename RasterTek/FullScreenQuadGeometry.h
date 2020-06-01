#pragma once

#include "Geometry.h"

class FullScreenQuadGeometry : public GeometryDerived<FullScreenQuadGeometry>
{
	void GenerateVertexData() override
	{
		VertexData vd;
		const unsigned int vertexCount = 4;

		XMFLOAT3 positions[vertexCount]
		{
			XMFLOAT3(-1.0f, -1.0f, 0.0f),
			XMFLOAT3(-1.0f,  1.0f, 0.0f),
			XMFLOAT3(1.0f, -1.0f, 0.0f),
			XMFLOAT3(1.0f,  1.0f, 0.0f)
		};

		XMFLOAT2 texture[vertexCount]
		{
			XMFLOAT2(0.0, 1.0),
			XMFLOAT2(0.0, 0.0),
			XMFLOAT2(1.0, 1.0),
			XMFLOAT2(1.0, 0.0)
		};

		for (int i = 0; i < vertexCount; ++i)
		{
			vd.position = positions[i];
			vd.normal = positions[i]; // bruh
			vd.textureUV = texture[i];

			vertices.push_back(vd);
		}

		indices = std::vector<USHORT>{ 0, 1, 2, 1, 3, 2 };
	}

public:

	using GeometryDerived::GeometryDerived;
};