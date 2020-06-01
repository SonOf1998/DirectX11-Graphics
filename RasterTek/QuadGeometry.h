#pragma once

#include "Geometry.h"

class QuadGeometry : public GeometryDerived<QuadGeometry>
{
	void GenerateVertexData() override
	{
		VertexData vd;
		const unsigned int vertexCount = 4;

		XMFLOAT3 positions[vertexCount]
		{
			XMFLOAT3(-0.5f, -0.5f, 0.0f),
			XMFLOAT3(-0.5f,  0.5f, 0.0f),
			XMFLOAT3( 0.5f, -0.5f, 0.0f),
			XMFLOAT3( 0.5f,  0.5f, 0.0f)
		};

		XMFLOAT3 colors[vertexCount]
		{
			XMFLOAT3( 1.0f,  0.0f, 0.0f),
			XMFLOAT3( 1.0f,  0.0f, 0.0f),
			XMFLOAT3( 1.0f,  0.0f, 0.0f),
			XMFLOAT3( 1.0f,  0.0f, 0.0f)
		};

		XMFLOAT3 normals[vertexCount]
		{
			XMFLOAT3(0.0f,  0.0f, 1.0f),
			XMFLOAT3(0.0f,  0.0f, 1.0f),
			XMFLOAT3(0.0f,  0.0f, 1.0f),
			XMFLOAT3(0.0f,  0.0f, 1.0f)
		};

		XMFLOAT2 texture[vertexCount]
		{
			XMFLOAT2(0.0, 50.0),
			XMFLOAT2(0.0, 0.0),
			XMFLOAT2(50.0, 50.0),
			XMFLOAT2(50.0, 0.0)
		};

		for (int i = 0; i < vertexCount; ++i)
		{
			vd.position = positions[i];
			vd.normal = normals[i];
			vd.textureUV = texture[i];

			vertices.push_back(vd);
		}

		indices = std::vector<USHORT>{ 0, 1, 2, 1, 3, 2 };
	}

public:

	using GeometryDerived::GeometryDerived;
};