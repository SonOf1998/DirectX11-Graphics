#pragma once

#include "pch.h"

#include "HResultException.h"
#include "VertexDataType.h"


struct Geometry
{
	virtual ~Geometry() = default;

	virtual void Draw(ID3D11DeviceContext*) = 0;
};

template <typename DATA_TYPE>
class GeometryDerived : public Geometry
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	

protected:
	
	std::vector<typename DATA_TYPE::DATA>   vertices;
	std::vector<USHORT>						indices;

	GeometryDerived() = default;


	virtual void GenerateVertexData() = 0;
	void CreateBuffers(ID3D11Device* dev)
	{
		// vertex buffer
		D3D11_BUFFER_DESC vertexBufferDesc		= {};
		D3D11_SUBRESOURCE_DATA vertexBufferData	= {};

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = DATA_TYPE::GetSizeInBytes() * vertices.size();
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = DATA_TYPE::GetSizeInBytes();

		vertexBufferData.pSysMem = vertices.data();
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;

		HRESULT result;
		THROW_IF_HRESULT_FAILED(dev->CreateBuffer(&vertexBufferDesc, &vertexBufferData, vertexBuffer.GetAddressOf()));


		// index buffer
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

	void Draw(ID3D11DeviceContext* devcon)
	{
		UINT stride = DATA_TYPE::GetSizeInBytes();
		UINT offset = 0;

		devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		devcon->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		devcon->DrawIndexed(indices.size(), 0, 0);
	}
};
