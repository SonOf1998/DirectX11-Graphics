#pragma once

class Texture
{
	ComPtr<ID3D11ShaderResourceView> texture;
	UINT bindingSlot;

public:

	Texture(ID3D11Device*, ID3D11DeviceContext*, LPCWSTR, UINT);
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	~Texture() = default;

	ID3D11ShaderResourceView* GetTexture();
	UINT GetBindingSlot() const;
};