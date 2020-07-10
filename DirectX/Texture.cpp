#include "pch.h"
#include "Texture.h"
#include "HResultException.h"

using namespace DirectX;

Texture::Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCWSTR filename, UINT bindingSlot) : bindingSlot(bindingSlot)
{
	HRESULT result;
	
	THROW_IF_HRESULT_FAILED(CreateWICTextureFromFile(device, deviceContext, filename, nullptr, texture.GetAddressOf()));

	// ComPtr<ID3D11Resource> Res;
	// if (lstrcmpW(filename, L"Textures/solid_wood.png") == 0)
	// 	SaveWICTextureToFile(deviceContext, Res.Get(), GUID_ContainerFormatBmp, L"C:/DX11/SCREENSHOT.BMP");
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return texture.Get();
}

UINT Texture::GetBindingSlot() const
{
	return bindingSlot;
}
