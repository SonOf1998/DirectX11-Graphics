#pragma once

class GameObject;
class Light;
class Renderable;
class RenderTargetTexture;
class OrthographicCamera;
class PerspectiveCamera;
class Pipeline;


#define PIPELINES_AND_OBJECTS										\
std::unique_ptr<Pipeline>				pipelinePhongBlinn;			\
std::unique_ptr<Pipeline>				pipelineMirror;				\
std::unique_ptr<Pipeline>				pipelineShadowMap;			\
std::unique_ptr<Pipeline>				pipelineLoDTess;			\
std::unique_ptr<Pipeline>				pipelineBezierQuad;			\
std::unique_ptr<Pipeline>				pipelineOverlay;			\
																	\
std::unique_ptr<Renderable>				ballSet;					\
std::unique_ptr<Renderable>				armchairSet;				\
std::unique_ptr<Renderable>				chairSet;					\
std::unique_ptr<Renderable>				overlaySet;					\



class Graphics
{
public:
	inline static UINT screenWidth;
	inline static UINT screenHeight;
private:
	HWND hwnd;

	ComPtr<IDXGISwapChain>			swapchain;
	ComPtr<ID3D11Device>			dev;
	ComPtr<ID3D11DeviceContext>		devcon;
	ComPtr<ID3D11RenderTargetView>	renderTargetView;
	ComPtr<ID3D11DepthStencilState> depthStencilState;
	ComPtr<ID3D11DepthStencilView>	depthStencilView;
	ComPtr<ID3D11BlendState>		defaultBlendState;
	ComPtr<ID3D11BlendState>		overlayBlendState;


	D3D11_VIEWPORT					viewport;

	std::unique_ptr<PerspectiveCamera>			camera;
	std::unique_ptr<OrthographicCamera>			overlayCamera;
	std::vector<std::unique_ptr<GameObject>>	gameObjects;

	PIPELINES_AND_OBJECTS

	
	std::unique_ptr<GameObject> mirror;
	std::unique_ptr<Light> dirLight;

	std::unique_ptr<RenderTargetTexture> shadowMap;

	void CreateSwapChain(IDXGIFactory1*);
	void CreateRenderTarget();
	void CreateAndSetRasterizerState();
	void CreateAndSetDepthStencilState();
	void CreateBlendStates();
	
	void InitializeDirectX11();
	void InitializeImGui();
	void RenderInitalization();



	void SetRenderTargetToBackBuffer();
	void SetRenderTargetToTexture(RenderTargetTexture&);

public:
	Graphics(UINT, UINT, HWND);
	Graphics(const Graphics&)	= delete;
	~Graphics();							// cpp fájlban defaultoljuk, mert ott már ismert a camera, gameobject ...

	IDXGIAdapter1* GetBestVideoCard(IDXGIFactory1*);

	void RenderFrame(float t, float dt);

	void SwitchMode(bool toFullscreen);
	void Resize(UINT width, UINT height);

	void ForwardWheelMessage(short deltaZ);
};

