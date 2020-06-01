#pragma once

class Camera;
class GameObject;
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
	ComPtr<ID3D11DepthStencilView>	depthStencilView;

	std::unique_ptr<Camera>						camera;
	std::vector<std::unique_ptr<GameObject>>	gameObjects;

	std::unique_ptr<ShaderProgram>				shaderProgramPhongBlinn;
	std::unique_ptr<ShaderProgram>				shaderProgramMirror;

	std::unique_ptr<GameObject> mirror;

	void CreateSwapChain(IDXGIFactory1*);
	void CreateRenderTarget();
	void CreateAndSetRasterizerState();
	void CreateAndSetDepthStencilState();
	void Initialize();

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

