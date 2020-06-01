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
	
};

struct P : public VertexDataStructure
{
	XMFLOAT3 position;
};

struct PC : public VertexDataStructure
{
	XMFLOAT3 position;
	XMFLOAT3 color;
};

struct PNT : public VertexDataStructure
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texcoord;
};