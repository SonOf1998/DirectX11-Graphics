#pragma once

#include <d3d11_4.h>

#include <string>


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
};
