#include "pch.h"
#include "HResultException.h"


HResultException::HResultException(const wchar_t* file, int line, HRESULT hr)
{
	_com_error error(hr);
	
	excContent += (std::wstring(file) + L"  (line: " + std::to_wstring(line) + L") :\t\t\t\t\n\n");
	excContent += std::wstring(error.ErrorMessage());
}

LPCWSTR HResultException::GetErrorMsg() const
{
	return excContent.c_str();
}
