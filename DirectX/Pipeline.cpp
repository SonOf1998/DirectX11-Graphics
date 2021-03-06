#include "pch.h"
#include "Pipeline.h"



#include "CBufferNotFoundException.h"
#include "HResultException.h"
#include "SimpleDXMath.h"
#include "Texture.h"

Pipeline::Pipeline(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCWSTR vertexShaderName, LPCWSTR pixelShaderName, LPCWSTR geometryShaderName,
							LPCWSTR hullShaderName, LPCWSTR domainShaderName, std::vector<D3D11_INPUT_ELEMENT_DESC> layout) : device(device), deviceContext(deviceContext)
{
	HRESULT result;

	ComPtr<ID3D10Blob> vertexShaderBinary;
	THROW_IF_HRESULT_FAILED(D3DReadFileToBlob(vertexShaderName, vertexShaderBinary.GetAddressOf()));;
	

	ComPtr<ID3D10Blob> pixelShaderBinary;
	if (pixelShaderName != nullptr)
	{
		THROW_IF_HRESULT_FAILED(D3DReadFileToBlob(pixelShaderName, pixelShaderBinary.GetAddressOf()));
	}

	ComPtr<ID3D10Blob> geometryShaderBinary;
	if (geometryShaderName != nullptr)
	{
		THROW_IF_HRESULT_FAILED(D3DReadFileToBlob(geometryShaderName, geometryShaderBinary.GetAddressOf()));
	}
	
	ComPtr<ID3D10Blob> hullShaderBinary;
	if (hullShaderName != nullptr)
	{
		THROW_IF_HRESULT_FAILED(D3DReadFileToBlob(hullShaderName, hullShaderBinary.GetAddressOf()));
	}

	ComPtr<ID3D10Blob> domainShaderBinary;
	if (domainShaderName != nullptr)
	{
		THROW_IF_HRESULT_FAILED(D3DReadFileToBlob(domainShaderName, domainShaderBinary.GetAddressOf()));
	}

	// Create the vertex shader from the buffer.
	THROW_IF_HRESULT_FAILED(device->CreateVertexShader(vertexShaderBinary->GetBufferPointer(), vertexShaderBinary->GetBufferSize(), nullptr, vertexShader.GetAddressOf()));

	// Create the pixel shader from the buffer.
	if (pixelShaderBinary != nullptr)
	{
		THROW_IF_HRESULT_FAILED(device->CreatePixelShader(pixelShaderBinary->GetBufferPointer(), pixelShaderBinary->GetBufferSize(), nullptr, pixelShader.GetAddressOf()));
	}

	if (geometryShaderBinary != nullptr)
	{
		THROW_IF_HRESULT_FAILED(device->CreateGeometryShader(geometryShaderBinary->GetBufferPointer(), geometryShaderBinary->GetBufferSize(), nullptr, geometryShader.GetAddressOf()));
	}
	
	// Create the hull shader from the buffer.
	if (hullShaderBinary != nullptr)
	{
		THROW_IF_HRESULT_FAILED(device->CreateHullShader(hullShaderBinary->GetBufferPointer(), hullShaderBinary->GetBufferSize(), nullptr, hullShader.GetAddressOf()));
	}

	// Create the pixel shader from the buffer.
	if (domainShaderBinary != nullptr)
	{
		THROW_IF_HRESULT_FAILED(device->CreateDomainShader(domainShaderBinary->GetBufferPointer(), domainShaderBinary->GetBufferSize(), nullptr, domainShader.GetAddressOf()));
	}
	
	// Create the vertex input layout.
	THROW_IF_HRESULT_FAILED(device->CreateInputLayout(layout.data(), static_cast<UINT>(layout.size()), vertexShaderBinary->GetBufferPointer(), vertexShaderBinary->GetBufferSize(), inputLayout.GetAddressOf()));
	InitShaderReflection(vertexShaderBinary.Get(), pixelShaderBinary.Get(), geometryShaderBinary.Get(), hullShaderBinary.Get(), domainShaderBinary.Get());
}

Pipeline::~Pipeline()
{	
	/*ComPtr<ID3D11Debug> debug;
	device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(debug.GetAddressOf()));
	debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);*/
}

void Pipeline::Use()
{
	deviceContext->IASetInputLayout(inputLayout.Get());	

	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	deviceContext->GSSetShader(geometryShader.Get(), nullptr, 0);
	deviceContext->HSSetShader(hullShader.Get(), nullptr, 0);
	deviceContext->DSSetShader(domainShader.Get(), nullptr, 0);
}

void Pipeline::SetCBuffer(CBufferDataType* cbufferData, CBUFFER_LOCATION location)
{
	std::string name = cbufferData->name;

	if (cbufferSlots.find(location) == cbufferSlots.end())
	{
		// location has no cbuffers whatsoever (or maybe optimized away..)
		throw CBufferNotFoundException(name);
	}

	if (cbufferSlots[location].find(name) == cbufferSlots[location].end())
	{
		// location has some cbuffers but not with the name we're looking for
		throw CBufferNotFoundException(name);
	}
	
	cbufferData->SetAsShaderCBuffer(device, deviceContext, location, cbufferSlots[location][name]);
}

void Pipeline::SetTexture(Texture* texture, TEXTURE_LOCATION location /* PIXEL SHADER */)
{
	ID3D11ShaderResourceView* resView = texture->GetTexture();

	switch (location)
	{
		case TEXTURE_LOCATION::VERTEX_SHADER_TEXTURE:
			deviceContext->VSSetShaderResources(texture->GetBindingSlot(), 1, &resView);
			break;
		case TEXTURE_LOCATION::HULL_SHADER_TEXTURE:
			deviceContext->HSSetShaderResources(texture->GetBindingSlot(), 1, &resView);
			break;
		case TEXTURE_LOCATION::DOMAIN_SHADER_TEXTURE:
			deviceContext->DSSetShaderResources(texture->GetBindingSlot(), 1, &resView);
			break;
		case TEXTURE_LOCATION::GEOMETRY_SHADER_TEXTURE:
			deviceContext->GSSetShaderResources(texture->GetBindingSlot(), 1, &resView);
			break;
		case TEXTURE_LOCATION::PIXEL_SHADER_TEXTURE:
			deviceContext->PSSetShaderResources(texture->GetBindingSlot(), 1, &resView);
			break;
		case TEXTURE_LOCATION::COMPUTE_SHADER_TEXTURE:
			deviceContext->CSSetShaderResources(texture->GetBindingSlot(), 1, &resView);
			break;
		default:
			throw;
	}
}

void Pipeline::SetTexture(ID3D11ShaderResourceView* texture, UINT bindingSlot, TEXTURE_LOCATION location)
{
	switch (location)
	{
		case TEXTURE_LOCATION::VERTEX_SHADER_TEXTURE:
			deviceContext->VSSetShaderResources(bindingSlot, 1, &texture);
			break;
		case TEXTURE_LOCATION::HULL_SHADER_TEXTURE:
			deviceContext->HSSetShaderResources(bindingSlot, 1, &texture);
			break;
		case TEXTURE_LOCATION::DOMAIN_SHADER_TEXTURE:
			deviceContext->DSSetShaderResources(bindingSlot, 1, &texture);
			break;
		case TEXTURE_LOCATION::GEOMETRY_SHADER_TEXTURE:
			deviceContext->GSSetShaderResources(bindingSlot, 1, &texture);
			break;
		case TEXTURE_LOCATION::PIXEL_SHADER_TEXTURE:
			deviceContext->PSSetShaderResources(bindingSlot, 1, &texture);
			break;
		case TEXTURE_LOCATION::COMPUTE_SHADER_TEXTURE:
			deviceContext->CSSetShaderResources(bindingSlot, 1, &texture);
			break;
		default:
			throw;
	}
}

void Pipeline::SetSampler(FILTERING filtering, UINT bindingSlot, SAMPLER_LOCATION location /* PIXEL SHADER */)
{
	HRESULT result;
	ComPtr<ID3D11SamplerState> samplerState;
	D3D11_SAMPLER_DESC samplerDesc = {};

	switch (filtering)
	{
		case FILTERING::NEAREST:
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			samplerDesc.MipLODBias = mipOffset;
			break;
		case FILTERING::BILINEAR:
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			samplerDesc.MipLODBias = mipOffset;
			break;
		case FILTERING::TRILINEAR:
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			samplerDesc.MipLODBias = mipOffset;
			break;
		case FILTERING::ANISOTROPIC_X4:
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samplerDesc.MaxAnisotropy = 4;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			samplerDesc.MipLODBias = mipOffset;
			break;
		case FILTERING::ANISOTROPIC_X16:
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samplerDesc.MaxAnisotropy = 16;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			samplerDesc.MipLODBias = mipOffset;
			break;
		default:
			throw;
	}
	
	THROW_IF_HRESULT_FAILED(device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf()));

	switch (location)
	{
		case SAMPLER_LOCATION::VERTEX_SHADER_SAMPLER:
			deviceContext->VSSetSamplers(bindingSlot, 1, samplerState.GetAddressOf());
			break;
		case SAMPLER_LOCATION::HULL_SHADER_SAMPLER:
			deviceContext->HSSetSamplers(bindingSlot, 1, samplerState.GetAddressOf());
			break;
		case SAMPLER_LOCATION::DOMAIN_SHADER_SAMPLER:
			deviceContext->DSSetSamplers(bindingSlot, 1, samplerState.GetAddressOf());
			break;
		case SAMPLER_LOCATION::GEOMETRY_SHADER_SAMPLER:
			deviceContext->GSSetSamplers(bindingSlot, 1, samplerState.GetAddressOf());
			break;
		case SAMPLER_LOCATION::PIXEL_SHADER_SAMPLER:
			deviceContext->PSSetSamplers(bindingSlot, 1, samplerState.GetAddressOf());
			break;
		case SAMPLER_LOCATION::COMPUTE_SHADER_SAMPLER:
			deviceContext->CSSetSamplers(bindingSlot, 1, samplerState.GetAddressOf());
			break;
		default:
			throw;
	}
}


/* Gathers every cbuffer's name and binding slot.
 * Unused cbuffers are sadly optimized away and will not be seen.
 * This might be turned off in hlsl settings.
 */
void Pipeline::InitShaderReflection(ID3D10Blob* vertexShaderBinary, ID3D10Blob* pixelShaderBinary, ID3D10Blob* geometryShaderBinary, ID3D10Blob* hullShaderBinary, ID3D10Blob* domainShaderBinary)
{
	HRESULT result;

	
	ComPtr<ID3D11ShaderReflection> shaderReflection;
	D3DReflect(vertexShaderBinary->GetBufferPointer(), vertexShaderBinary->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(shaderReflection.GetAddressOf()));
	D3D11_SHADER_DESC shaderDesc;
	THROW_IF_HRESULT_FAILED(shaderReflection->GetDesc(&shaderDesc));

	std::map<std::string, UINT> cbufferNameSlotPairs;
	auto gatherShaderReflectionData = [&]() {
		cbufferNameSlotPairs.clear();
		for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
		{
			ID3D11ShaderReflectionConstantBuffer* constBufferReflection = shaderReflection->GetConstantBufferByIndex(i);
			D3D11_SHADER_BUFFER_DESC constBufferDesc;
			THROW_IF_HRESULT_FAILED(constBufferReflection->GetDesc(&constBufferDesc));
			LPCSTR constBufferName = constBufferDesc.Name;
			D3D11_SHADER_INPUT_BIND_DESC inputBindDesc;
			THROW_IF_HRESULT_FAILED(shaderReflection->GetResourceBindingDescByName(constBufferName, &inputBindDesc));
			cbufferNameSlotPairs.insert(std::pair<std::string, UINT>(std::string(constBufferName), inputBindDesc.BindPoint));
		}
	};


	// VERTEX SHADER REFLECTION
	// Other parts can be found above the lambda definition
	gatherShaderReflectionData();
	if (!cbufferNameSlotPairs.empty())
	{
		cbufferSlots.insert(std::pair<CBUFFER_LOCATION, std::map<std::string, UINT>>(CBUFFER_LOCATION::VERTEX_SHADER_CBUFFER, cbufferNameSlotPairs));
	}	

	//PIXEL SHADER REFLECTION
	if (pixelShaderBinary != nullptr)
	{
		D3DReflect(pixelShaderBinary->GetBufferPointer(), pixelShaderBinary->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(shaderReflection.ReleaseAndGetAddressOf()));
		THROW_IF_HRESULT_FAILED(shaderReflection->GetDesc(&shaderDesc));

		gatherShaderReflectionData();		
		if (!cbufferNameSlotPairs.empty())
		{
			cbufferSlots.insert(std::pair<CBUFFER_LOCATION, std::map<std::string, UINT>>(CBUFFER_LOCATION::PIXEL_SHADER_CBUFFER, cbufferNameSlotPairs));
		}
	}

	//GEOMETRY SHADER REFLECTION
	if (geometryShaderBinary != nullptr)
	{
		D3DReflect(geometryShaderBinary->GetBufferPointer(), geometryShaderBinary->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(shaderReflection.ReleaseAndGetAddressOf()));
		THROW_IF_HRESULT_FAILED(shaderReflection->GetDesc(&shaderDesc));

		gatherShaderReflectionData();
		if (!cbufferNameSlotPairs.empty())
		{
			cbufferSlots.insert(std::pair<CBUFFER_LOCATION, std::map<std::string, UINT>>(CBUFFER_LOCATION::GEOMETRY_SHADER_CBUFFER, cbufferNameSlotPairs));
		}
	}

	//HULL SHADER REFLECTION
	if (hullShaderBinary != nullptr)
	{
		D3DReflect(hullShaderBinary->GetBufferPointer(), hullShaderBinary->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(shaderReflection.ReleaseAndGetAddressOf()));
		THROW_IF_HRESULT_FAILED(shaderReflection->GetDesc(&shaderDesc));

		gatherShaderReflectionData();
		if (!cbufferNameSlotPairs.empty())
		{
			cbufferSlots.insert(std::pair<CBUFFER_LOCATION, std::map<std::string, UINT>>(CBUFFER_LOCATION::HULL_SHADER_CBUFFER, cbufferNameSlotPairs));
		}
	}
		
	//DOMAIN SHADER REFLECTION
	if (domainShaderBinary != nullptr)
	{
		D3DReflect(domainShaderBinary->GetBufferPointer(), domainShaderBinary->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(shaderReflection.ReleaseAndGetAddressOf()));
		THROW_IF_HRESULT_FAILED(shaderReflection->GetDesc(&shaderDesc));

		gatherShaderReflectionData();
		if (!cbufferNameSlotPairs.empty())
		{
			cbufferSlots.insert(std::pair<CBUFFER_LOCATION, std::map<std::string, UINT>>(CBUFFER_LOCATION::DOMAIN_SHADER_CBUFFER, cbufferNameSlotPairs));
		}
	}
}
