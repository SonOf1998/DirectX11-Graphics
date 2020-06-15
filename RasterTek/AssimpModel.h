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
			auto vertex = mesh->mVertices[i];
			auto normal = mesh->mNormals[i];
			auto texcoord = mesh->mTextureCoords[0][i];

			DATA_TYPE vertexData;

			if (vertexData.HandlesPosition())
			{
				vertexData.SetPosition(XMFLOAT3(static_cast<float>(vertex.x), static_cast<float>(vertex.y), static_cast<float>(vertex.z)));
			}
			if (vertexData.HandlesNormal())
			{
				vertexData.SetNormal(XMFLOAT3(static_cast<float>(normal.x), static_cast<float>(normal.y), static_cast<float>(normal.z)));
			}
			if (vertexData.HandlesTexcoordUV())
			{
				vertexData.SetTexcoordUV(XMFLOAT2(static_cast<float>(texcoord.x), static_cast<float>(texcoord.y)));
			}

			this->vertices.push_back(vertexData.GetData());
		}

		this->indices.reserve(mesh->mNumFaces * 3);
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			auto face = mesh->mFaces[i];
			this->indices.push_back(face.mIndices[0]);
			this->indices.push_back(face.mIndices[1]);
			this->indices.push_back(face.mIndices[2]);
		}
	}
};