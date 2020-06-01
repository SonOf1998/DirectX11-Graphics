#pragma once

#include "pch.h"

#include "HResultException.h"
#include "VertexDataType.h"


class AssimpModel;

struct Geometry
{
	virtual ~Geometry() = default;

	virtual void Draw(ID3D11DeviceContext*) = 0;
};

template <class DERIVED = nullptr_t>
class GeometryDerived : public Geometry
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	

protected:

	struct VertexData
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 textureUV;
	};

	std::vector<VertexData> vertices;
	std::vector<USHORT>		indices;

	GeometryDerived() = default;


	virtual void GenerateVertexData() = 0;
	void CreateBuffers(ID3D11Device* dev)
	{
		D3D11_BUFFER_DESC vertexBufferDesc		= {};
		D3D11_SUBRESOURCE_DATA vertexBufferData	= {};

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexData) * vertices.size();
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = sizeof(VertexData);

		vertexBufferData.pSysMem = vertices.data();
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;

		HRESULT result;
		THROW_IF_HRESULT_FAILED(dev->CreateBuffer(&vertexBufferDesc, &vertexBufferData, vertexBuffer.GetAddressOf()));

		D3D11_BUFFER_DESC indexBufferDesc		= {};
		D3D11_SUBRESOURCE_DATA indexBufferData	= {};

		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned short) * indices.size();
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = sizeof(unsigned short);

		indexBufferData.pSysMem = indices.data();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;

		THROW_IF_HRESULT_FAILED(dev->CreateBuffer(&indexBufferDesc, &indexBufferData, indexBuffer.GetAddressOf()));
	}
	

public:

	/* For user defined geometries.
	*
	* These are the usually simple polygons or parametrized surfaces.
	*/
	template <typename DATA_TYPE>
	static DERIVED* Create(ID3D11Device* dev)
	{
		static_assert(!std::is_same_v<DERIVED, nullptr_t>, "CreateGeometry called with abstract base class!");
		static_assert(!std::is_same_v<DERIVED, AssimpModel>, "For model loading use ::Create(dev, filename, meshindex)");
  		static_assert(std::is_base_of_v<VertexDataStructure, DATA_TYPE>, "Invalid data type use P PNT PNTT etc.");

		GeometryDerived* ret = new DERIVED();
		ret->GenerateVertexData();
		ret->CreateBuffers(dev);

		return reinterpret_cast<DERIVED*>(ret);
	}

	/* For loading complex models.
	* A model might contain multiple meshes.
	*
	*/
	template<typename DATA_TYPE>
	static DERIVED* Create(ID3D11Device* dev, const char* modelFileName, unsigned int meshIndex)
	{
		static_assert(std::is_same_v<DERIVED, AssimpModel>, "Use single paramtered ::Create for simple geometries");
		static_assert(std::is_base_of_v<VertexDataStructure, DATA_TYPE>, "Invalid data type");
		
		DERIVED* ret = new DERIVED();
		ret->GenerateVertexData(modelFileName, meshIndex);
		ret->CreateBuffers(dev);

		return ret;
	}
		

	void Draw(ID3D11DeviceContext* devcon)
	{
		UINT stride = sizeof(VertexData);
		UINT offset = 0;

		devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		devcon->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		devcon->DrawIndexed(indices.size(), 0, 0);
	}
};
