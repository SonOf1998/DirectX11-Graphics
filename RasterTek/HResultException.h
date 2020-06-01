#pragma once


#define THROW_IF_HRESULT_FAILED(hrcall) if(FAILED(result = (hrcall))) throw HResultException(__FILEW__, __LINE__, result);

class HResultException : public std::exception
{
	std::wstring excContent;

public:

	HResultException(const wchar_t* file, int line, HRESULT hr);
	LPCWSTR GetErrorMsg() const;
};