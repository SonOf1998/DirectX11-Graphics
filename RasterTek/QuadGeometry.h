#pragma once

#include "Geometry.h"

template <typename DATA_TYPE>
class QuadGeometry : public GeometryDerived<DATA_TYPE>
{
	void GenerateVertexData() override
	{
		DATA_TYPE vertexData;
		const unsigned int vertexCount = 4;

		XMFLOAT3 positions[vertexCount]
		{
			XMFLOAT3(-0.5f, -0.5f, 0.0f),
			XMFLOAT3(-0.5f,  0.5f, 0.0f),
			XMFLOAT3( 0.5f, -0.5f, 0.0f),
			XMFLOAT3( 0.5f,  0.5f, 0.0f)
		};

		XMFLOAT3 normals[vertexCount]
		{
			XMFLOAT3(0.0f,  0.0f, 1.0f),
			XMFLOAT3(0.0f,  0.0f, 1.0f),
			XMFLOAT3(0.0f,  0.0f, 1.0f),
			XMFLOAT3(0.0f,  0.0f, 1.0f)
		};

		XMFLOAT2 texcoordUV[vertexCount]
		{
			XMFLOAT2(0.0, 50.0),
			XMFLOAT2(0.0, 0.0),
			XMFLOAT2(50.0, 50.0),
			XMFLOAT2(50.0, 0.0)
		};

		for (int i = 0; i < vertexCount; ++i)
		{
			if (vertexData.HandlesPosition())
			{
				vertexData.SetPosition(positions[i]);
			}
			if (vertexData.HandlesNormal())
			{
				vertexData.SetNormal(normals[i]);
			}
			if (vertexData.HandlesTexcoordUV())
			{
				vertexData.SetTexcoordUV(texcoordUV[i]);
			}

			this->vertices.push_back(vertexData.GetData());
		}

		this->indices = std::vector<USHORT>{ 0, 1, 2, 1, 3, 2 };
	}

public:

	QuadGeometry(ID3D11Device* device) : GeometryDerived<DATA_TYPE>()
	{
		GenerateVertexData();
		this->CreateBuffers(device);
	}
};