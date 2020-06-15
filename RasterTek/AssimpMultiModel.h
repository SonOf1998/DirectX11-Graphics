#pragma once

#include "GameObject.h"
#include "Geometry.h"

/* Use for importing complex Maya / Blender model files
*  This method creates multiple Mesh pointers with a single
*  read. Textures and materials need to be added separately!
*
*  This class assumes single instance of UV mapping
*/

template <typename DATA_TYPE>
class AssimpMultiModel
{
	class AssimpMultiModelFragment : public GeometryDerived<DATA_TYPE>
	{
		friend class AssimpMultiModel<DATA_TYPE>;

		void GenerateVertexData() {}
	};
	   
	std::vector<std::shared_ptr<AssimpMultiModelFragment>> geometries;

public:

	AssimpMultiModel(ID3D11Device* device, const char* filename)
	{
		GenerateVertexData(device, filename);
	}

	void GenerateVertexData(ID3D11Device* device, const char* fileName)
	{
		Assimp::Importer importer;
		auto model = importer.ReadFile(fileName, aiProcess_JoinIdenticalVertices | aiProcess_FlipWindingOrder | aiProcess_PreTransformVertices);
		for (uint i = 0; i < model->mNumMeshes; ++i)
		{
			auto mesh = model->mMeshes[i];

			std::shared_ptr<AssimpMultiModelFragment> fragment = std::make_shared<AssimpMultiModelFragment>();
			fragment->vertices.reserve(mesh->mNumVertices);

			for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
			{
				auto vertex = mesh->mVertices[j];
				auto normal = mesh->mNormals[j];
				auto texcoord = mesh->mTextureCoords[0][j];

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

				fragment->vertices.push_back(vertexData.GetData());
			}

			fragment->indices.reserve(mesh->mNumFaces * 3);
			for (uint j = 0; j < mesh->mNumFaces; ++j)
			{
				auto face = mesh->mFaces[j];
				fragment->indices.push_back(face.mIndices[0]);
				fragment->indices.push_back(face.mIndices[1]);
				fragment->indices.push_back(face.mIndices[2]);
			}

			fragment->CreateBuffers(device);
			geometries.push_back(std::move(fragment));
		}
	}

	std::vector<std::shared_ptr<AssimpMultiModelFragment>>& GetGeometries()
	{
		return geometries;
	}
};