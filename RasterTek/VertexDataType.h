#pragma once

#include "pch.h"

using namespace DirectX;

namespace DataTypes
{
	class DataType
	{
	protected:

		D3D11_INPUT_ELEMENT_DESC desc;

	public:

		virtual ~DataType() = default;
		const D3D11_INPUT_ELEMENT_DESC& GetDesc()
		{
			return desc;
		}
	};

	// PER VERTEX //

	struct Position : public DataType
	{
		Position()
		{
			desc.SemanticName = "Position";
			desc.SemanticIndex = 0;
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			desc.InputSlot = 0;
			desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
		}
	};

	struct Color : public DataType
	{
		Color()
		{
			desc.SemanticName = "COLOR";
			desc.SemanticIndex = 0;
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			desc.InputSlot = 0;
			desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
		}
	};

	struct Normal : public DataType
	{
		Normal()
		{
			desc.SemanticName = "NORMAL";
			desc.SemanticIndex = 0;
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			desc.InputSlot = 0;
			desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
		}
	};

	struct Texture : public DataType
	{
		Texture()
		{
			desc.SemanticName = "TEXCOORD";
			desc.SemanticIndex = 0;
			desc.Format = DXGI_FORMAT_R32G32_FLOAT;
			desc.InputSlot = 0;
			desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
		}
	};

	// PER INSTANCE //

	// none so far..
}

template<typename... TYPES>
struct VertexDataType;

template<>
class VertexDataType<> 
{
protected:

	std::vector<D3D11_INPUT_ELEMENT_DESC> semanticFormatList;

public:

	std::vector<D3D11_INPUT_ELEMENT_DESC>& GetSemanticFormatList()
	{
		return semanticFormatList;
	}
	virtual ~VertexDataType() = default;
};


template<typename HEAD, typename... TAIL>
class VertexDataType<HEAD, TAIL...> : public VertexDataType<TAIL...>
{

public:

	VertexDataType()
	{
		HEAD dataType;
		this->semanticFormatList.push_back(dataType.GetDesc());
	}
};

using InputLayoutP   = typename VertexDataType<DataTypes::Position>;
using InputLayoutPC  = typename VertexDataType<DataTypes::Color, DataTypes::Position>;
using InputLayoutPT  = typename VertexDataType<DataTypes::Texture, DataTypes::Position>;
using InputLayoutPNT = typename VertexDataType<DataTypes::Texture, DataTypes::Normal, DataTypes::Position>;



/*
* position
* color
* normal
* texcoord
* tangent
* binormal
*/



struct VertexDataStructure
{	
	virtual ~VertexDataStructure() = default;

	virtual bool HandlesPosition()		{ return false; }
	virtual bool HandlesColor()			{ return false; }
	virtual bool HandlesNormal()		{ return false; }
	virtual bool HandlesTexcoordUV()	{ return false; }
	
	virtual void SetPosition	(const XMFLOAT3&) { throw std::runtime_error("Cannot set Position");	}
	virtual void SetColor		(const XMFLOAT3&) { throw std::runtime_error("Cannot set Color");		}
	virtual void SetNormal		(const XMFLOAT3&) { throw std::runtime_error("Cannot set Normal");		}
	virtual void SetTexcoordUV	(const XMFLOAT2&) { throw std::runtime_error("Cannot set TexcoordUV");	}
};

struct P : public VertexDataStructure
{
	struct DATA
	{
		XMFLOAT3 position;
	};

private:

	DATA data;
	
public:
	static size_t GetSizeInBytes()  { return sizeof(DATA); }
	
	bool HandlesPosition()  final	{ return true; }

	void SetPosition(const XMFLOAT3& position) final
	{
		data.position = position;
	}

	DATA GetData()
	{
		return data;
	}
};

struct PC : public VertexDataStructure
{
	struct DATA
	{
		XMFLOAT3 position;
		XMFLOAT3 color;
	};

private:

	DATA data;
	
public:
	static size_t GetSizeInBytes()  { return sizeof(DATA); }

	bool HandlesPosition()  final	{ return true; }
	bool HandlesColor()     final	{ return true; }

	void SetPosition(const XMFLOAT3& position) final
	{
		data.position = position;
	}

	void SetColor(const XMFLOAT3& color) final
	{
		data.color = color;
	}

	DATA GetData()
	{
		return data;
	}
};

struct PT : public VertexDataStructure
{
	struct DATA
	{
		XMFLOAT3 position;
		XMFLOAT2 texcoordUV;
	};

private:

	DATA data;

public:
	
	static size_t GetSizeInBytes()  { return sizeof(DATA); }

	bool HandlesPosition()   final	{ return true; }
	bool HandlesTexcoordUV() final	{ return true; }

	void SetPosition(const XMFLOAT3& position) final
	{
		data.position = position;
	}

	void SetTexcoordUV(const XMFLOAT2& texcoordUV) final
	{
		data.texcoordUV = texcoordUV;
	}

	DATA GetData()
	{
		return data;
	}
};

struct PNT : public VertexDataStructure
{
	struct DATA
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 texcoordUV;
	};

private:

	DATA data;

public:
	
	static size_t GetSizeInBytes() { return sizeof(DATA); }

	bool HandlesPosition()   final  { return true; }
	bool HandlesNormal()	 final	{ return true; }
	bool HandlesTexcoordUV() final	{ return true; }

	void SetPosition(const XMFLOAT3& position) final
	{
		data.position = position;
	}

	void SetNormal(const XMFLOAT3& normal) final
	{
		data.normal = normal;
	}

	void SetTexcoordUV(const XMFLOAT2& texcoordUV) final
	{
		data.texcoordUV = texcoordUV;
	}

	DATA GetData()
	{
		return data;
	}
};