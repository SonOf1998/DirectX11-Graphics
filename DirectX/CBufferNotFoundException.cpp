#pragma once
#include "pch.h"
#include "CBufferNotFoundException.h"

CBufferNotFoundException::CBufferNotFoundException(const std::string& name)
{
	nameOfSearchedCBuffer = name;
}

const char * CBufferNotFoundException::what() const
{
	excContent = "The cbuffer you wanted to bind cannot be found or might not be actively used by any statement contributing to the output (" + nameOfSearchedCBuffer + ")";	
	return excContent.c_str();
}
