#pragma once

#include "Geometry.h"

class AssimpModel : public GeometryDerived<AssimpModel>
{
	void GenerateVertexData() override { /*empty*/ }

public:

	using GeometryDerived::GeometryDerived;

	void GenerateVertexData(const char* fileName, unsigned int meshIndex)
	{
		Assimp::Importer importer;
		auto model = importer.ReadFile(fileName, aiProcess_JoinIdenticalVertices | aiProcess_FlipWindingOrder);
		auto mesh = model->mMeshes[meshIndex];

		vertices.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			auto vertex = mesh->mVertices[i];
			auto normal = mesh->mNormals[i];
			auto texcoord = mesh->mTextureCoords[0][i];

			VertexData vd;
			vd.position = XMFLOAT3(static_cast<float>(vertex.x), static_cast<float>(vertex.y), static_cast<float>(vertex.z));
			vd.normal = XMFLOAT3(static_cast<float>(normal.x), static_cast<float>(normal.y), static_cast<float>(normal.z));
			vd.textureUV = XMFLOAT2(static_cast<float>(texcoord.x), static_cast<float>(texcoord.y));
			vertices.push_back(vd);
		}

		indices.reserve(mesh->mNumFaces * 3);
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			auto face = mesh->mFaces[i];
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}
	}
};