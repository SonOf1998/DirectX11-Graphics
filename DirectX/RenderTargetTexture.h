#pragma once

// equivalent of OpenGL's framebuffer
// we write it through render target view
// we use it as texture2d by getting its shader res view

class RenderTargetTexture
{
	ComPtr<ID3D11RenderTargetView>   renderTargetView;
	ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	ComPtr<ID3D11DepthStencilState>	 depthStencilState;
	ComPtr<ID3D11DepthStencilView>	 depthStencilView;
	D3D11_VIEWPORT					 viewport;


	void CreateAndSetRenderTargetView(ID3D11Device*, UINT, UINT, DXGI_FORMAT);
	void CreateAndSetDepthStencilState(ID3D11Device*, UINT, UINT, DXGI_FORMAT);

public:
	RenderTargetTexture(ID3D11Device*, UINT, UINT, DXGI_FORMAT);
	~RenderTargetTexture() = default;

	ID3D11RenderTargetView*		GetRenderTargetView();
	ID3D11RenderTargetView**	GetRenderTargetViewAddress();
	ID3D11ShaderResourceView*	GetShaderResourceView();
	ID3D11DepthStencilState*	GetDepthStencilState();
	ID3D11DepthStencilView*		GetDepthStencilView();
	D3D11_VIEWPORT				GetViewport() const;
};

