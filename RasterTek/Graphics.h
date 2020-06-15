#pragma once

class Camera;
class GameObject;
class Light;
class RenderTargetTexture;
class ShaderProgram;

class Graphics
{
	UINT screenWidth;
	UINT screenHeight;
	HWND hwnd;

	ComPtr<IDXGISwapChain>			swapchain;
	ComPtr<ID3D11Device>			dev;
	ComPtr<ID3D11DeviceContext>		devcon;
	ComPtr<ID3D11RenderTargetView>	renderTargetView;
	ComPtr<ID3D11DepthStencilState> depthStencilState;
	ComPtr<ID3D11DepthStencilView>	depthStencilView;

	D3D11_VIEWPORT					viewport;

	std::unique_ptr<Camera>						camera;
	std::vector<std::unique_ptr<GameObject>>	gameObjects;

	std::unique_ptr<ShaderProgram>				shaderProgramPhongBlinn;
	std::unique_ptr<ShaderProgram>				shaderProgramMirror;
	std::unique_ptr<ShaderProgram>				shaderProgramShadowMap;

	std::unique_ptr<GameObject> mirror;
	std::unique_ptr<Light> dirLight;

	std::unique_ptr<RenderTargetTexture> shadowMap;

	void CreateSwapChain(IDXGIFactory1*);
	void CreateRenderTarget();
	void CreateAndSetRasterizerState();
	void CreateAndSetDepthStencilState();
	void RenderInitalization();
	void InitializeDirectX11();

	void SetRenderTargetToBackBuffer();
	void SetRenderTargetToTexture(RenderTargetTexture&);

public:
	Graphics(UINT, UINT, HWND);
	Graphics(const Graphics&)	= delete;
	~Graphics();							// cpp fájlban defaultoljuk, mert ott már ismert a camera, gameobject ...

	IDXGIAdapter1* GetBestVideoCard(IDXGIFactory1*);

	void RenderFrame(float, float);
	void Resize(UINT, UINT);
};

