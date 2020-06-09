#include "pch.h"
#include "Graphics.h"


#include "HResultException.h"
#include "Geometry.h"
#include "AssimpModel.h"
#include "QuadGeometry.h"
#include "Logger.h"
#include "DirectionalLight.h"
#include "VertexDataType.h"
#include "Mesh.h"
#include "GameObject.h"
#include "SphereObject.h"
#include "FloorObject.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "RenderTargetTexture.h"
#include "FullScreenQuadObject.h"
#include "FullScreenQuadGeometry.h"

Graphics::Graphics(UINT screenWidth, UINT screenHeight, HWND hwnd) : screenWidth(screenWidth), screenHeight(screenHeight), hwnd(hwnd)
{
	InitializeDirectX11();

	camera = std::make_unique<Camera>(XMVectorSet(0, 1, 4, 1), XMVectorSet(0, 0, 0, 1), static_cast<float>(screenWidth) / screenHeight);



	std::unique_ptr<GameObject> plane = std::make_unique<FloorObject>(dev.Get(), devcon.Get(), XMVectorSet(0, -1, 0, 0), XMVectorSet(10, 10, 1, 1), XMVectorSet(1, 0, 0, 0), -XM_PI / 2);
	Texture* planeTexture = new Texture(dev.Get(), devcon.Get(), L"C:/DX11/RasterTek/RasterTek/Textures/pavement.jpg", 0);
	Geometry* planeGeometry = new QuadGeometry<PNT>(dev.Get());
	Material* planeMaterial = new Material{ XMFLOAT3(0.1f,0.1f,0.1f), XMFLOAT3(0,0,0), 0.0f };
	Mesh* planeMesh = new Mesh(planeGeometry, planeMaterial);
	planeMesh->SetTexture(planeTexture);
	plane->AddMesh(planeMesh);
	

	std::unique_ptr<GameObject> monkey = std::make_unique<SphereObject>(dev.Get(), devcon.Get(), XMVectorSet(0, 0.5, 0, 0));
	std::shared_ptr<Texture> monkeyTexture = new Texture(dev.Get(), devcon.Get(), L"C:/DX11/RasterTek/RasterTek/Textures/monkey.jpg", 0);
	Geometry* monkeyGeometry = new AssimpModel<PNT>(dev.Get(), "Models/sphere.obj");
	Material* monkeyMaterial = new Material{ XMFLOAT3(0.1f,0.1f,0.1f), XMFLOAT3(1,1,1), 50.0f };
	Mesh* monkeyMesh = new Mesh(monkeyGeometry, monkeyMaterial);
	monkeyMesh->SetTexture(monkeyTexture);
	monkey->AddMesh(monkeyMesh);

	std::unique_ptr<GameObject> torus = std::make_unique<SphereObject>(dev.Get(), devcon.Get(), XMVectorSet(3.0, 0.3, 0.0, 0.0), XMVectorSet(1, 1, 1, 1));
	Geometry* torusGeometry = new AssimpModel<PNT>(dev.Get(), "Models/torus.obj");
	Material* torusMaterial = new Material{ XMFLOAT3(0.1f,0.1f,0.1f), XMFLOAT3(1,0,0), 100.0f };
	Mesh* torusMesh = new Mesh(torusGeometry, torusMaterial);
	torusMesh->SetTexture(monkeyTexture);
	torus->AddMesh(torusMesh);

	std::unique_ptr<GameObject> fullScreenQuad = std::make_unique<FullScreenQuadObject>(dev.Get(), devcon.Get(), XMVectorSet(0, 0, 0, 1), XMVectorSet(1, 1, 1, 1));
	Geometry* fullScreenQuadGeometry = new FullScreenQuadGeometry<PT>(dev.Get());
	Mesh* fullScreenMesh = new Mesh(fullScreenQuadGeometry);
	fullScreenQuad->AddMesh(fullScreenMesh);
	mirror = std::move(fullScreenQuad);

	//for (int i = 0; i < 7; ++i)
	//{
	//	std::unique_ptr<GameObject> nanosuit = std::make_unique<GameObject>(dev.Get(), devcon.Get(), XMVectorSet(3, 0, -5, 0), XMVectorSet(0.1, 0.1, 0.1, 1));
	//	Texture* nanosuitTexture = new Texture(dev.Get(), devcon.Get(), L"C:/DX11/RasterTek/RasterTek/Textures/monkey.jpg", 0);
	//	Geometry* nanosuitGeometry = AssimpModel::Create<PNT>(dev.Get(), "Models/nanosuit.obj", i);
	//	Material* nanosuitMaterial = new Material{ XMFLOAT3(0.1,0.1,0.1), XMFLOAT3(1,1,1), 50.0f };
	//	Mesh* nanosuitMesh = new Mesh(nanosuitGeometry, nanosuitMaterial);
	//	nanosuitMesh->SetTexture(nanosuitTexture);
	//	nanosuit->addMesh(nanosuitMesh);
	//	gameObjects.push_back(std::move(nanosuit));
	//}
	
	
	

	gameObjects.push_back(std::move(plane));
	gameObjects.push_back(std::move(monkey));
	gameObjects.push_back(std::move(torus));
	
	dirLight = std::make_unique<DirectionalLight>(XMFLOAT3(6, 8, 3), XMFLOAT3(1,1,1));
	shadowMap.reset(new RenderTargetTexture(dev.Get(), screenWidth, screenHeight, DXGI_FORMAT_R32_FLOAT));
	
	shaderProgramPhongBlinn.reset(ShaderProgram::Create<InputLayoutPNT>(dev.Get(), devcon.Get(), L"Shaders/Phong-Blinn/bin/vs.cso", L"Shaders/Phong-Blinn/bin/ps.cso"));
	shaderProgramMirror.reset(ShaderProgram::Create<InputLayoutPT>(dev.Get(), devcon.Get(), L"Shaders/FullScreenQuadPT/bin/vs.cso", L"Shaders/FullScreenQuadPT/bin/ps.cso"));
	shaderProgramMirror->SetSampler(FILTERING::NEAREST, 0);
	shaderProgramShadowMap.reset(ShaderProgram::Create<InputLayoutP>(dev.Get(), devcon.Get(), L"Shaders/ShadowMap-Directional/bin/vs.cso", L"Shaders/ShadowMap-Directional/bin/ps.cso"));
}


void Graphics::RenderFrame(float t, float dt)
{
	float color[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float white[4] = { 10.0f, 10.0f, 10.0f, 1.0f };

	// Rendering to texture //
	//RenderTargetTexture rtt(dev.Get(), screenWidth / 10, screenHeight / 10, DXGI_FORMAT_R32G32B32A32_FLOAT);
	SetRenderTargetToTexture(*(shadowMap.get()));
	devcon->ClearRenderTargetView(shadowMap->GetRenderTargetView(), white);
	devcon->ClearDepthStencilView(shadowMap->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	camera->Animate(t, dt);

	for (auto& gameObject : gameObjects)
	{
		gameObject->Animate(t, dt);
		gameObject->RenderToShadowMap(devcon.Get(), shaderProgramShadowMap.get(), dirLight.get());
	}


	// Rendering to backbuffer //
	SetRenderTargetToBackBuffer();
	devcon->ClearRenderTargetView(renderTargetView.Get(), color);
	devcon->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	camera->Animate(t, dt);


	shaderProgramPhongBlinn->Use();
	shaderProgramPhongBlinn->SetTexture(shadowMap->GetShaderResourceView(), 1);
	for (auto& gameObject : gameObjects)
	{
		gameObject->Animate(t, dt);
		gameObject->Render(devcon.Get(), shaderProgramPhongBlinn.get(), camera.get(), dirLight.get());
	}

	if (VSYNC_ENABLED)
	{
		swapchain->Present(1, 0);
	}
	else
	{
		swapchain->Present(0, 0);
	}
}


Graphics::~Graphics() = default;

void Graphics::CreateSwapChain(IDXGIFactory1* factory)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	UINT quality;
	dev->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, MSAALevel, &quality);

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (VSYNC_ENABLED)
	{
		// FPS korl�t
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 20;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	else
	{
		// MAX BLASZT
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = MSAALevel;
	swapChainDesc.SampleDesc.Quality = quality - 1;

	if (FULLSCREEN)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	HRESULT result;
	THROW_IF_HRESULT_FAILED(factory->CreateSwapChain(dev.Get(), &swapChainDesc, swapchain.GetAddressOf()));
}

void Graphics::CreateRenderTarget()
{
	ComPtr<ID3D11Texture2D> backBuffer;

	HRESULT result;
	THROW_IF_HRESULT_FAILED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
	THROW_IF_HRESULT_FAILED(dev->CreateRenderTargetView(backBuffer.Get(), NULL, renderTargetView.ReleaseAndGetAddressOf()));
}

void Graphics::CreateAndSetRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;


	ComPtr<ID3D11RasterizerState> rasterState;
	HRESULT result;
	THROW_IF_HRESULT_FAILED(dev->CreateRasterizerState(&rasterDesc, rasterState.GetAddressOf()));

	// Now set the rasterizer state.
	devcon->RSSetState(rasterState.Get());
}

void Graphics::CreateAndSetDepthStencilState()
{
	HRESULT result;

	// depth map
	D3D11_TEXTURE2D_DESC depthBufferDesc = {};

	UINT quality;
	dev->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, MSAALevel, &(quality));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = MSAALevel;
	depthBufferDesc.SampleDesc.Quality = quality - 1;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	
	ComPtr<ID3D11Texture2D> depthStencilBuffer;
	THROW_IF_HRESULT_FAILED(dev->CreateTexture2D(&depthBufferDesc, NULL, depthStencilBuffer.GetAddressOf()));
	

	
	// Z testing
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

	
	THROW_IF_HRESULT_FAILED(dev->CreateDepthStencilState(&depthStencilDesc, depthStencilState.ReleaseAndGetAddressOf()));
	   
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	
	THROW_IF_HRESULT_FAILED(dev->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, depthStencilView.ReleaseAndGetAddressOf()));
}
 
void Graphics::InitializeDirectX11()
{
	HRESULT result;

	ComPtr<IDXGIFactory1> factory;
	THROW_IF_HRESULT_FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf())));

	// Create device and devicecontext
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;	
	THROW_IF_HRESULT_FAILED(D3D11CreateDevice (
		GetBestVideoCard(factory.Get()),
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG, 
		&featureLevel, 1, 
		D3D11_SDK_VERSION, 
		dev.GetAddressOf(),
		nullptr, 
		devcon.GetAddressOf()
	));

	CreateSwapChain(factory.Get());
	CreateRenderTarget();
	CreateAndSetDepthStencilState();
	CreateAndSetRasterizerState();

	viewport.Width = static_cast<float>(screenWidth);
	viewport.Height = static_cast<float>(screenHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	devcon->RSSetViewports(1, &viewport);
}

void Graphics::SetRenderTargetToBackBuffer()
{
	devcon->RSSetViewports(1, &viewport);
	devcon->OMSetDepthStencilState(depthStencilState.Get(), 0);
	devcon->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
}

void Graphics::SetRenderTargetToTexture(RenderTargetTexture& renderTargetTexture)
{
	devcon->RSSetViewports(1, &(renderTargetTexture.GetViewport()));
	devcon->OMSetDepthStencilState(renderTargetTexture.GetDepthStencilState(), 0);
	devcon->OMSetRenderTargets(1, renderTargetTexture.GetRenderTargetViewAddress(), renderTargetTexture.GetDepthStencilView());
}

IDXGIAdapter1* Graphics::GetBestVideoCard(IDXGIFactory1* factory)
{
	IDXGIAdapter1* adapter;

	IDXGIAdapter1* ret = nullptr;
	unsigned long maxVideoMemory = 0;

	if (CreateDXGIFactory1(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory)) == S_OK)
	{
		DXGI_ADAPTER_DESC videoCardDesc;
		for (int i = 0; factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			adapter->GetDesc(&videoCardDesc);

			if (videoCardDesc.DedicatedVideoMemory > maxVideoMemory)
			{
				maxVideoMemory = videoCardDesc.DedicatedVideoMemory;
				ret = adapter;
			}
		}
	}

	return ret;
}

void Graphics::Resize(UINT newWidth, UINT newHeight)
{
	HRESULT result;
	
	screenWidth = newWidth;
	screenHeight = newHeight;
	camera->SetAspectRatio(static_cast<float>(screenWidth) / screenHeight);

	//devcon->ClearState();
	devcon->OMSetRenderTargets(0, nullptr, nullptr);
	renderTargetView.ReleaseAndGetAddressOf();

	THROW_IF_HRESULT_FAILED(swapchain->ResizeBuffers(1, newWidth, newHeight, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	CreateRenderTarget();
	CreateAndSetDepthStencilState();

	// Set up the viewport.
	viewport.Width  = static_cast<float>(newWidth);
	viewport.Height = static_cast<float>(newHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	devcon->RSSetViewports(1, &viewport);
}




