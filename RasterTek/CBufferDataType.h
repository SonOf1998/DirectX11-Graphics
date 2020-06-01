#pragma once

#include "pch.h"

#include "SharedEnums.h"
#include "HResultException.h"

struct CBufferDataType
{	
protected:
	CBufferDataType(const char* name) : name(name) {}

public:
	const std::string name;

	virtual ~CBufferDataType() = default;
	virtual void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource) = 0;
	virtual size_t GetDataSize() = 0;

	virtual void SetAsShaderCBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, CBUFFER_LOCATION location, UINT bindingSlot)
	{
		HRESULT result;

		D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = GetDataSize();
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		ComPtr<ID3D11Buffer> matrixBuffer;
		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		THROW_IF_HRESULT_FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, matrixBuffer.GetAddressOf()));

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		// open cbuffer for writing
		THROW_IF_HRESULT_FAILED(deviceContext->Map(matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		FillCBufferWithData(mappedResource);	// template method

		deviceContext->Unmap(matrixBuffer.Get(), 0);

		switch (location)
		{
			case CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER:
				deviceContext->VSSetConstantBuffers(bindingSlot, 1, matrixBuffer.GetAddressOf());
				break;
			case CBUFFER_LOCATION::HULL_SHADER_CBUFFER:
				deviceContext->HSSetConstantBuffers(bindingSlot, 1, matrixBuffer.GetAddressOf());
				break;
			case CBUFFER_LOCATION::DOMAIN_SHADER_CBUFFER:
				deviceContext->DSSetConstantBuffers(bindingSlot, 1, matrixBuffer.GetAddressOf());
				break;
			case CBUFFER_LOCATION::GEOMETRY_SHADER_CBUFFER:
				deviceContext->GSSetConstantBuffers(bindingSlot, 1, matrixBuffer.GetAddressOf());
				break;
			case CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER:
				deviceContext->PSSetConstantBuffers(bindingSlot, 1, matrixBuffer.GetAddressOf());
				break;
			case CBUFFER_LOCATION::COMPUTE_SHADER_CBUFFER:
				deviceContext->CSSetConstantBuffers(bindingSlot, 1, matrixBuffer.GetAddressOf());
				break;
			default:
				throw;
		}
	}
};

struct MVP : public CBufferDataType
{
	struct MVPData
	{
		XMMATRIX model;
		XMMATRIX viewProj;
	} data;

	
	MVP() : CBufferDataType("MVP")
	{

	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource) override
	{
		MVPData* cbufferData = reinterpret_cast<MVPData*>(mappedResource.pData);
		cbufferData->model    = data.model;
		cbufferData->viewProj = data.viewProj;
	}

	size_t GetDataSize()
	{
		return sizeof(data);
	}
	
	MVPData& GetData()
	{
		return data;
	}
};

struct MMInv : public CBufferDataType
{
	struct MMInvData
	{
		XMMATRIX model;
		XMMATRIX modelInv;
	} data;

	MMInv() : CBufferDataType("MMInv")
	{

	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource)
	{
		MMInvData* cbufferData = reinterpret_cast<MMInvData*>(mappedResource.pData);
		cbufferData->model = data.model;
		cbufferData->modelInv = data.modelInv;
	}

	size_t GetDataSize()
	{
		return sizeof(data);
	}

	MMInvData& GetData()
	{
		return data;
	}
};

struct MVPMInv : public CBufferDataType
{
	struct MVPMInvData 
	{
		XMMATRIX model;
		XMMATRIX viewProj;
		XMMATRIX modelInv;
	} data;

	MVPMInv() : CBufferDataType("MVPMInv")
	{

	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource)
	{
		MVPMInvData* cbufferData = reinterpret_cast<MVPMInvData*>(mappedResource.pData);
		cbufferData->model = data.model;
		cbufferData->viewProj = data.viewProj;
		cbufferData->modelInv = data.modelInv;
	}

	size_t GetDataSize()
	{
		return sizeof(data);
	}

	MVPMInvData& GetData()
	{
		return data;
	}
};

struct ViewProj : public CBufferDataType
{
	struct VPData
	{
		XMMATRIX viewProj;
	} data;

	ViewProj() : CBufferDataType("VP")
	{
	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource)
	{
		VPData* cbufferData = reinterpret_cast<VPData*>(mappedResource.pData);
		cbufferData->viewProj = data.viewProj;
	}
	
	size_t GetDataSize()
	{
		return sizeof(data);
	}

	VPData& GetData()
	{
		return data;
	}
};

struct Material : public CBufferDataType
{
	struct MaterialData
	{
		__declspec(align(16)) XMFLOAT3 ambient;
		__declspec(align(16)) XMFLOAT3 specular;
		__declspec(align(16)) float shininess;

		MaterialData(XMFLOAT3 ambient, XMFLOAT3 specular, float shininess) : ambient(ambient), specular(specular), shininess(shininess) {}
	} data;

	Material(XMFLOAT3 ambient, XMFLOAT3 specular, float shininess) : CBufferDataType("Material"), data(ambient, specular, shininess)
	{

	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource)
	{
		MaterialData* cbufferData = reinterpret_cast<MaterialData*>(mappedResource.pData);
		cbufferData->ambient = data.ambient;
		cbufferData->specular = data.specular;
		cbufferData->shininess = data.shininess;
	}

	size_t GetDataSize()
	{
		return sizeof(data);
	}

	MaterialData& GetData()
	{
		return data;
	}
};

struct CameraCBuffer : public CBufferDataType
{
	struct CameraData
	{
		XMVECTOR position;

		CameraData(const XMVECTOR& position) : position(position) {}
	} data;

	CameraCBuffer(XMVECTOR position) : CBufferDataType("Camera"), data(position)
	{

	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource)
	{
		CameraData* cbufferData = reinterpret_cast<CameraData*>(mappedResource.pData);
		cbufferData->position = data.position;
	}

	size_t GetDataSize()
	{
		return sizeof(data);
	}

	CameraData& GetData()
	{
		return data;
	}
};