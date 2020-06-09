#pragma once

#include "pch.h"

using namespace DirectX;

namespace DataTypes
{
	struct Position
	{
		const char* semantic = "POSITION";
		DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;
	};

	struct Color
	{
		const char* semantic = "COLOR";
		DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;
	};

	struct Normal
	{
		const char* semantic = "NORMAL";
		DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;
	};

	struct Texture
	{
		const char* semantic = "TEXCOORD";
		DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT;
	};
}

template<typename... TYPES>
struct VertexDataType;

template<>
struct VertexDataType<> 
{
	std::vector<std::pair<const char*, DXGI_FORMAT>> semanticFormatList;
	virtual ~VertexDataType() = default;
};


template<typename HEAD, typename... TAIL>
struct VertexDataType<HEAD, TAIL...> : public VertexDataType<TAIL...>
{
	VertexDataType()
	{
		HEAD dataType;
		this->semanticFormatList.push_back(std::make_pair(dataType.semantic, dataType.format));
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