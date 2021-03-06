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
	virtual UINT GetDataSize() = 0;

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

struct M : public CBufferDataType
{
	struct MData
	{
		XMMATRIX model;
	} data;


	M() : CBufferDataType("M")
	{

	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource) override
	{
		MData* cbufferData = reinterpret_cast<MData*>(mappedResource.pData);
		cbufferData->model = data.model;
	}

	UINT GetDataSize()
	{
		return sizeof(data);
	}

	MData& GetData()
	{
		return data;
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

	UINT GetDataSize()
	{
		return sizeof(data);
	}
	
	MVPData& GetData()
	{
		return data;
	}
};

struct BB : public CBufferDataType
{
	struct BBData
	{
		XMVECTOR rightVector;
		XMVECTOR upVector;
	} data;


	BB() : CBufferDataType("BB")
	{

	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource) override
	{
		BBData* cbufferData = reinterpret_cast<BBData*>(mappedResource.pData);
		cbufferData->rightVector = data.rightVector;
		cbufferData->upVector = data.upVector;
	}

	UINT GetDataSize()
	{
		return sizeof(data);
	}

	BBData& GetData()
	{
		return data;
	}
};

// will be used for instancing
template <size_t size = 1>
struct MMInv : public CBufferDataType
{
	struct MMInvData
	{
		XMMATRIX model[size];
		XMMATRIX modelInv[size];
	} data;

	MMInv() : CBufferDataType("MMInv")
	{

	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource)
	{
		MMInvData* cbufferData = reinterpret_cast<MMInvData*>(mappedResource.pData);
		for (size_t i = 0; i < size; ++i)
		{
			cbufferData->model[i] = data.model[i];
			cbufferData->modelInv[i] = data.modelInv[i];
		}
	}

	UINT GetDataSize()
	{
		return size * sizeof(data);
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

	UINT GetDataSize()
	{
		return sizeof(data);
	}

	MVPMInvData& GetData()
	{
		return data;
	}
};

struct VP : public CBufferDataType
{
	struct VPData
	{
		XMMATRIX viewProj;
	} data;

	VP() : CBufferDataType("VP")
	{
	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource)
	{
		VPData* cbufferData = reinterpret_cast<VPData*>(mappedResource.pData);
		cbufferData->viewProj = data.viewProj;
	}
	
	UINT GetDataSize()
	{
		return sizeof(data);
	}

	VPData& GetData()
	{
		return data;
	}
};

struct Proj : public CBufferDataType
{
	struct ProjData
	{
		XMMATRIX proj;
	} data;

	Proj() : CBufferDataType("P")
	{
	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource)
	{
		ProjData* cbufferData = reinterpret_cast<ProjData*>(mappedResource.pData);
		cbufferData->proj = data.proj;
	}

	UINT GetDataSize()
	{
		return sizeof(data);
	}

	ProjData& GetData()
	{
		return data;
	}
};

struct DirLightVP : public CBufferDataType
{
	struct DirLightVPData
	{
		XMMATRIX viewProj;
	} data;

	DirLightVP() : CBufferDataType("DirLightVP")
	{
	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource)
	{
		DirLightVPData* cbufferData = reinterpret_cast<DirLightVPData*>(mappedResource.pData);
		cbufferData->viewProj = data.viewProj;
	}

	UINT GetDataSize()
	{
		return sizeof(data);
	}

	DirLightVPData& GetData()
	{
		return data;
	}
};

struct Material : public CBufferDataType
{
	/* Notice: 
	*
	*  Setting the shininess component to zero might cause
	*  visual artifacts like black patches.
	*
	*  Seems like 0 on the power of 0 is calculated to -inf
	*  in the pipeline's shader programs.
	*/
	struct MaterialData
	{
		__declspec(align(16)) XMFLOAT3 ambient;
		__declspec(align(16)) XMFLOAT3 specular;
		__declspec(align(16)) float shininess;

		MaterialData(XMFLOAT3 ambient, XMFLOAT3 specular, float shininess) : ambient(ambient), specular(specular), shininess(shininess) {}
	} data;

	Material(XMFLOAT3 ambient, XMFLOAT3 specular, float shininess) : CBufferDataType("Material"), data(ambient, specular, shininess)
	{
		assert(shininess != 0.0);
	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource)
	{
		MaterialData* cbufferData = reinterpret_cast<MaterialData*>(mappedResource.pData);
		cbufferData->ambient = data.ambient;
		cbufferData->specular = data.specular;
		cbufferData->shininess = data.shininess;
	}

	UINT GetDataSize()
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
	} data;

	CameraCBuffer() : CBufferDataType("Camera")
	{

	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource)
	{
		CameraData* cbufferData = reinterpret_cast<CameraData*>(mappedResource.pData);
		cbufferData->position = data.position;
	}

	UINT GetDataSize()
	{
		return sizeof(data);
	}

	CameraData& GetData()
	{
		return data;
	}
};


struct Centroid : public CBufferDataType
{
	struct CentroidData
	{
		XMVECTOR position;
	} data;

	Centroid() : CBufferDataType("Centroid")
	{

	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource)
	{
		CentroidData* cbufferData = reinterpret_cast<CentroidData*>(mappedResource.pData);
		cbufferData->position = data.position;
	}

	UINT GetDataSize()
	{
		return sizeof(data);
	}

	CentroidData& GetData()
	{
		return data;
	}
};

struct LOD : public CBufferDataType
{
	struct LODData
	{
		XMVECTOR maxTessFactor;
	} data;

	LOD() : CBufferDataType("LOD")
	{

	}

	void FillCBufferWithData(D3D11_MAPPED_SUBRESOURCE& mappedResource)
	{
		LODData* cbufferData = reinterpret_cast<LODData*>(mappedResource.pData);
		cbufferData->maxTessFactor = data.maxTessFactor;
	}

	UINT GetDataSize()
	{
		return sizeof(data);
	}

	LODData& GetData()
	{
		return data;
	}
};