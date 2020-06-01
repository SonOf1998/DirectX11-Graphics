#include "pch.h"
#include "Texture.h"

#include "HResultException.h"

using namespace DirectX;

Texture::Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCWSTR filename, UINT bindingSlot) : bindingSlot(bindingSlot)
{
	HRESULT result;
	THROW_IF_HRESULT_FAILED(CreateWICTextureFromFile(device, deviceContext, filename, nullptr, texture.GetAddressOf(), -1));
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return texture.Get();
}

UINT Texture::GetBindingSlot() const
{
	return bindingSlot;
}
