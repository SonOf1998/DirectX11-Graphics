#include "pch.h"
#include "RenderTargetTexture.h"


void RenderTargetTexture::CreateAndSetRenderTargetView(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format)
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

void RenderTargetTexture::CreateAndSetDepthStencilState(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format)
{
	HRESULT result;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	THROW_IF_HRESULT_FAILED(device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf()));


	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> depthStencilBuffer;
	THROW_IF_HRESULT_FAILED(device->CreateTexture2D(&depthBufferDesc, NULL, depthStencilBuffer.GetAddressOf()));
	

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	THROW_IF_HRESULT_FAILED(device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, depthStencilView.ReleaseAndGetAddressOf()));
}

RenderTargetTexture::RenderTargetTexture(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format)
{
	CreateAndSetRenderTargetView(device, width, height, format);
	CreateAndSetDepthStencilState(device, width, height, format);

	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
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

ID3D11DepthStencilState* RenderTargetTexture::GetDepthStencilState()
{
	return depthStencilState.Get();
}

ID3D11DepthStencilView* RenderTargetTexture::GetDepthStencilView()
{
	return depthStencilView.Get();
}

D3D11_VIEWPORT RenderTargetTexture::GetViewport() const
{
	return viewport;
}

