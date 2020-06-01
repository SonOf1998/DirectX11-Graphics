#pragma once

#include "pch.h"

class CBufferNotFoundException : public std::exception
{
	std::string nameOfSearchedCBuffer;
	mutable std::string excContent;

public:

	CBufferNotFoundException(const std::string&);
	const char* what() const override;
};