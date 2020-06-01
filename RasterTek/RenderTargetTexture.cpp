#include "pch.h"
#include "RenderTargetTexture.h"


RenderTargetTexture::RenderTargetTexture(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format)
{
	HRESULT result;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;


	ComPtr<ID3D11Texture2D> texture;
	THROW_IF_HRESULT_FAILED(device->CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf()));

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	renderTargetViewDesc.Format = format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	THROW_IF_HRESULT_FAILED(device->CreateRenderTargetView(texture.Get(), &renderTargetViewDesc, renderTargetView.GetAddressOf()));

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	THROW_IF_HRESULT_FAILED(device->CreateShaderResourceView(texture.Get(), &shaderResourceViewDesc, shaderResourceView.GetAddressOf()));
}

ID3D11RenderTargetView * RenderTargetTexture::GetRenderTargetView()
{
	return renderTargetView.Get();
}

ID3D11RenderTargetView** RenderTargetTexture::GetRenderTargetViewAddress()
{
	return renderTargetView.GetAddressOf();
}

ID3D11ShaderResourceView* RenderTargetTexture::GetShaderResourceView()
{
	return shaderResourceView.Get();
}

