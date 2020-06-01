#pragma once

// equivalent of OpenGL's framebuffer
// we write it through render target view
// we use it as texture2d by getting its shader res view

class RenderTargetTexture
{
	ComPtr<ID3D11RenderTargetView>   renderTargetView;
	ComPtr<ID3D11ShaderResourceView> shaderResourceView;

public:
	RenderTargetTexture(ID3D11Device*, UINT, UINT, DXGI_FORMAT);
	~RenderTargetTexture() = default;

	ID3D11RenderTargetView*		GetRenderTargetView();
	ID3D11RenderTargetView**	GetRenderTargetViewAddress();
	ID3D11ShaderResourceView*	GetShaderResourceView();
};

