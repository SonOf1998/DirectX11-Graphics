#pragma once

#include "Geometry.h"

/* Use for importing single mesh Maya / Blender files
*  or to draw only one specific mesh from a multimesh file. 
*/

template <typename DATA_TYPE>
class AssimpModel : public GeometryDerived<DATA_TYPE>
{
	void GenerateVertexData() override { /*empty*/ }

public:

	AssimpModel(ID3D11Device* device, const char* filename, UINT meshIndex = 0)
	{
		GenerateVertexData(filename, meshIndex);
		this->CreateBuffers(device);
	}

	void GenerateVertexData(const char* fileName, unsigned int meshIndex)
	{
		Assimp::Importer importer;
		auto model = importer.ReadFile(fileName, aiProcess_JoinIdenticalVertices | aiProcess_FlipWindingOrder | aiProcess_PreTransformVertices);
		auto mesh = model->mMeshes[meshIndex];

		this->vertices.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			DATA_TYPE vertexData;

			if (vertexData.HandlesPosition())
			{
				auto vertex = mesh->mVertices[i];

				float x = static_cast<float>(vertex.x);
				float y = static_cast<float>(vertex.y);
				float z = static_cast<float>(vertex.z);

				vertexData.SetPosition(XMFLOAT3(x, y, z));

				if (x < this->xMin)
				{
					this->xMin = x;
				}
				if (x > this->xMax)
				{
					this->xMax = x;
				}

				if (y < this->yMin)
				{
					this->yMin = y;
				}
				if (y > this->yMax)
				{
					this->yMax = y;
				}

				if (z < this->zMin)
				{
					this->zMin = z;
				}
				if (z > this->zMax)
				{
					this->zMax = z;
				}
			}
			if (vertexData.HandlesNormal())
			{
				auto normal = mesh->mNormals[i];
				vertexData.SetNormal(XMFLOAT3(static_cast<float>(normal.x), static_cast<float>(normal.y), static_cast<float>(normal.z)));
			}
			if (vertexData.HandlesTexcoordUV())
			{
				auto texcoord = mesh->mTextureCoords[0][i];
				vertexData.SetTexcoordUV(XMFLOAT2(static_cast<float>(texcoord.x), static_cast<float>(texcoord.y)));
			}

			this->vertices.push_back(vertexData.GetData());
		}

		uint topologyVertexCount = mesh->mFaces[0].mNumIndices;
		this->indices.reserve(mesh->mNumFaces * topologyVertexCount);
		for (uint j = 0; j < mesh->mNumFaces; ++j)
		{
			auto face = mesh->mFaces[j];
			for (uint k = 0; k < topologyVertexCount; ++k)
			{
				this->indices.push_back(face.mIndices[k]);
			}
		}
	}
};