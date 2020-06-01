#pragma once

class Camera;
class GameObject;
class ShaderProgram;

class Graphics
{
	static constexpr UINT	MSAALevel		= 4;
	static constexpr bool	FULLSCREEN		= false;
	static constexpr bool	VSYNC_ENABLED	= true;

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
	std::unique_ptr<ShaderProgram>				shaderProgram;

	void CreateSwapChain(IDXGIFactory1*);
	void CreateRenderTarget();
	void CreateAndSetRasterizerState();
	void CreateAndSetDepthStencilState();
	void Initialize();

public:
	Graphics(UINT, UINT, HWND);
	Graphics(const Graphics&)	= delete;
	~Graphics();							// cpp fájlban defaultoljuk, mert ott már ismert a camera, gameobject ...

	IDXGIAdapter1* GetBestVideoCard(IDXGIFactory1*);

	void RenderFrame(float, float);
	void Resize(UINT, UINT);
};

