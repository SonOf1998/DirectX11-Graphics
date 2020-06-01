#pragma once

#include "Geometry.h"

class CubeGeometry : public GeometryDerived<CubeGeometry>
{
	void GenerateVertexData() override
	{
		VertexData vd;
		const unsigned int vertexCount = 24;

		XMFLOAT3 positions[vertexCount]
		{
			// front
			XMFLOAT3(-0.5f, -0.5f,  0.5f),
			XMFLOAT3(-0.5f,  0.5f,  0.5f),
			XMFLOAT3( 0.5f, -0.5f,  0.5f),
			XMFLOAT3( 0.5f,  0.5f,  0.5f),
			// back
			XMFLOAT3( 0.5f, -0.5f, -0.5f),
			XMFLOAT3( 0.5f,  0.5f, -0.5f),
			XMFLOAT3(-0.5f, -0.5f, -0.5f),
			XMFLOAT3(-0.5f,  0.5f, -0.5f),
			// right
			XMFLOAT3( 0.5f, -0.5f,  0.5f),
			XMFLOAT3( 0.5f,  0.5f,  0.5f),
			XMFLOAT3( 0.5f, -0.5f, -0.5f),
			XMFLOAT3( 0.5f,  0.5f, -0.5f),
			// left					    
			XMFLOAT3(-0.5f, -0.5f, -0.5f),
			XMFLOAT3(-0.5f,  0.5f, -0.5f),
			XMFLOAT3(-0.5f, -0.5f,  0.5f),
			XMFLOAT3(-0.5f,  0.5f,  0.5f),
			// top					    
			XMFLOAT3(-0.5f,  0.5f,  0.5f),
			XMFLOAT3(-0.5f,  0.5f, -0.5f),
			XMFLOAT3( 0.5f,  0.5f,  0.5f),
			XMFLOAT3( 0.5f,  0.5f, -0.5f),
			// bottom					    
			XMFLOAT3( 0.5f, -0.5f,  0.5f),
			XMFLOAT3( 0.5f, -0.5f, -0.5f),
			XMFLOAT3(-0.5f, -0.5f,  0.5f),
			XMFLOAT3(-0.5f, -0.5f, -0.5f)
		};

		XMFLOAT3 colors[vertexCount]
		{
			XMFLOAT3(1.0f, 0.0f, 0.0f),
			XMFLOAT3(1.0f, 0.0f, 0.0f),
			XMFLOAT3(1.0f, 0.0f, 0.0f),
			XMFLOAT3(1.0f, 0.0f, 0.0f),

			XMFLOAT3(0.0f, 1.0f, 0.0f),
			XMFLOAT3(0.0f, 1.0f, 0.0f),
			XMFLOAT3(0.0f, 1.0f, 0.0f),
			XMFLOAT3(0.0f, 1.0f, 0.0f),

			XMFLOAT3(0.0f, 0.0f, 1.0f),
			XMFLOAT3(0.0f, 0.0f, 1.0f),
			XMFLOAT3(0.0f, 0.0f, 1.0f),
			XMFLOAT3(0.0f, 0.0f, 1.0f),

			XMFLOAT3(1.0f, 1.0f, 0.0f),
			XMFLOAT3(1.0f, 1.0f, 0.0f),
			XMFLOAT3(1.0f, 1.0f, 0.0f),
			XMFLOAT3(1.0f, 1.0f, 0.0f),

			XMFLOAT3(1.0f, 0.0f, 1.0f),
			XMFLOAT3(1.0f, 0.0f, 1.0f),
			XMFLOAT3(1.0f, 0.0f, 1.0f),
			XMFLOAT3(1.0f, 0.0f, 1.0f),

			XMFLOAT3(0.0f, 1.0f, 1.0f),
			XMFLOAT3(0.0f, 1.0f, 1.0f),
			XMFLOAT3(0.0f, 1.0f, 1.0f),
			XMFLOAT3(0.0f, 1.0f, 1.0f)
		};

		for (int i = 0; i < vertexCount; ++i)
		{
			//vd.position = positions[i];
			//vd.color = colors[i];
			//
			//vertices.push_back(vd);
		}

		indices = std::vector<USHORT>
		{ 
			0,  1,  2,  1,  3,  2,
			4,  5,  6,  5,  7,  6,
			8,  9,  10, 9,  11, 10,
			12, 13, 14, 13, 15, 14,
			16, 17, 18, 17, 19, 18,
			20, 21, 22, 21, 23, 22		
		};
	}

public:

	using GeometryDerived::GeometryDerived;
};