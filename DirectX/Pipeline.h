#pragma once

#include "CBufferDataType.h"
#include "SharedEnums.h"
#include "VertexDataType.h"

class Texture;

class Pipeline
{
	ComPtr<ID3D11VertexShader>		vertexShader;
	ComPtr<ID3D11HullShader>		hullShader;
	ComPtr<ID3D11DomainShader>		domainShader;
	ComPtr<ID3D11GeometryShader>	geometryShader;
	ComPtr<ID3D11PixelShader>		pixelShader;

	ComPtr<ID3D11InputLayout>  inputLayout;

	ID3D11Device*		 device;
	ID3D11DeviceContext* deviceContext;

	// for every shader we determine
	// the cbuffers in it (and save their names and slots as key-value pairs) 
	std::map<CBUFFER_LOCATION, std::map<std::string, UINT>> cbufferSlots;


	Pipeline(ID3D11Device*, ID3D11DeviceContext*, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, std::vector<D3D11_INPUT_ELEMENT_DESC>);

public:
	
	Pipeline(const Pipeline&)	= delete;
	~Pipeline();

	void Use();
	
	template <typename PER_VERTEX_DATA_TYPE>
	static Pipeline* Create(ID3D11Device* dev, ID3D11DeviceContext* devcon, 
    LPCWSTR vertexShaderFile, LPCWSTR pixelShaderFile = nullptr, LPCWSTR geometryShaderFile = nullptr, LPCWSTR hullShaderFile = nullptr, LPCWSTR domainShaderFile = nullptr)
	{
		// find or expand the possibilities at VertexDataType.h
		static_assert(std::is_base_of_v<VertexDataType<>, PER_VERTEX_DATA_TYPE>);
		PER_VERTEX_DATA_TYPE instance;
		Pipeline* ret = new Pipeline(dev, devcon, vertexShaderFile, pixelShaderFile, geometryShaderFile, hullShaderFile, domainShaderFile, instance.GetSemanticFormatList());
		return ret;
	}

	
	void SetCBuffer(CBufferDataType*, CBUFFER_LOCATION);
	void SetTexture(Texture*, TEXTURE_LOCATION = TEXTURE_LOCATION::PIXEL_SHADER_TEXTURE);
	void SetTexture(ID3D11ShaderResourceView*, UINT, TEXTURE_LOCATION = TEXTURE_LOCATION::PIXEL_SHADER_TEXTURE);
	void SetSampler(FILTERING, UINT, SAMPLER_LOCATION = SAMPLER_LOCATION::PIXEL_SHADER_SAMPLER);

private:

	void InitShaderReflection(ID3D10Blob*, ID3D10Blob*, ID3D10Blob*, ID3D10Blob*, ID3D10Blob*);
};

