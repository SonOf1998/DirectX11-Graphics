#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class Logger
{
public:
	
	static void print(std::string str)
	{
		str += '\n';
		OutputDebugStringA(str.c_str());
	}

	static void print(const wchar_t* str)
	{
		print(std::string(str, str + wcslen(str)));
	}

	static void print(const XMFLOAT3& vec)
	{
		print(std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + "\n");
	}
};
