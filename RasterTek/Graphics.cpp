#include "pch.h"
#include "Graphics.h"


#include "HResultException.h"
#include "Geometry.h"
#include "AssimpModel.h"
#include "AssimpMultiModel.h"
#include "QuadGeometry.h"
#include "Logger.h"
#include "DirectionalLight.h"
#include "VertexDataType.h"
#include "Mesh.h"
#include "GameObject.h"
#include "BallObject.h"
#include "FloorObject.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "RenderTargetTexture.h"
#include "FullScreenQuadObject.h"
#include "FullScreenQuadGeometry.h"

Graphics::Graphics(UINT screenWidth, UINT screenHeight, HWND hwnd) : screenWidth(screenWidth), screenHeight(screenHeight), hwnd(hwnd)
{
	InitializeDirectX11();
	RenderInitalization();	
}

void Graphics::RenderInitalization()
{
	camera = std::make_unique<Camera>(XMVectorSet(0, 1, 4, 1), XMVectorSet(0, 0, 0, 1), static_cast<float>(screenWidth) / screenHeight);

	// snooker balls - common resources (shape, shininess, colors)
	std::shared_ptr<Geometry> ballGeometry = std::make_shared<AssimpModel<PNT>>(dev.Get(), "Models/ball.obj");
	std::shared_ptr<Material> ballMaterial = std::make_shared<Material>(XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(1, 1, 1), 70.0f);
	std::shared_ptr<Texture>  redBallTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/redball.png", 0);
	std::shared_ptr<Texture>  yellowBallTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/yellowball.png", 0);
	std::shared_ptr<Texture>  greenBallTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/greenball.png", 0);
	std::shared_ptr<Texture>  brownBallTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/brownball.png", 0);
	std::shared_ptr<Texture>  blueBallTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/blueball.png", 0);
	std::shared_ptr<Texture>  pinkBallTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/pinkball.png", 0);
	std::shared_ptr<Texture>  blackBallTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/blackball.png", 0);
	std::shared_ptr<Texture>  whiteBallTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/whiteball.png", 0);
	Mesh ballMesh(ballGeometry, ballMaterial);
	float y = 0.96f;
	float x = 0.0f;
	float z = 0.0f;

	std::unique_ptr<GameObject> whiteBall = std::make_unique<BallObject>(dev.Get(), devcon.Get(), XMVectorSet(x + 1, y, z + 3, 0), XMVectorSet(0.1, 0.1, 0.1, 1));
	ballMesh.SetTexture(whiteBallTexture);
	whiteBall->CopyAndAddMesh(ballMesh);

	std::unique_ptr<GameObject> yellowBall = std::make_unique<BallObject>(dev.Get(), devcon.Get(), XMVectorSet(x + 1.37, y, z + 2.7, 0), XMVectorSet(0.1, 0.1, 0.1, 1));
	ballMesh.SetTexture(yellowBallTexture);
	yellowBall->CopyAndAddMesh(ballMesh);

	std::unique_ptr<GameObject> greenBall = std::make_unique<BallObject>(dev.Get(), devcon.Get(), XMVectorSet(x - 1.37, y, z + 2.7, 0), XMVectorSet(0.1, 0.1, 0.1, 1));
	ballMesh.SetTexture(greenBallTexture);
	greenBall->CopyAndAddMesh(ballMesh);

	std::unique_ptr<GameObject> brownBall = std::make_unique<BallObject>(dev.Get(), devcon.Get(), XMVectorSet(x, y, z + 2.7, 0), XMVectorSet(0.1, 0.1, 0.1, 1));
	ballMesh.SetTexture(brownBallTexture);
	brownBall->CopyAndAddMesh(ballMesh);

	std::unique_ptr<GameObject> blueBall = std::make_unique<BallObject>(dev.Get(), devcon.Get(), XMVectorSet(x, y, z, 0), XMVectorSet(0.1, 0.1, 0.1, 1));
	ballMesh.SetTexture(blueBallTexture);
	blueBall->CopyAndAddMesh(ballMesh);

	std::unique_ptr<GameObject> pinkBall = std::make_unique<BallObject>(dev.Get(), devcon.Get(), XMVectorSet(x, y, z -2.3, 0), XMVectorSet(0.1, 0.1, 0.1, 1));
	ballMesh.SetTexture(pinkBallTexture);
	pinkBall->CopyAndAddMesh(ballMesh);


	std::unique_ptr<GameObject> blackBall = std::make_unique<BallObject>(dev.Get(), devcon.Get(), XMVectorSet(x, y, z - 4.3, 0), XMVectorSet(0.1, 0.1, 0.1, 1));
	ballMesh.SetTexture(blackBallTexture);
	blackBall->CopyAndAddMesh(ballMesh);

	XMFLOAT4 firstRedTranslation(x, y, -2.5f, 0.0f);
	float deltaTranslate = 0.2f;
	for (int i = 0; i < 5; ++i)
	{
		float x0 = firstRedTranslation.x - ((i % 2) / 2.0f) * deltaTranslate;
		for (int j = 0; j <= i; ++j)
		{
			XMFLOAT4 translate = firstRedTranslation;
			translate.z -= i * deltaTranslate * 0.85f;
			translate.x = x0 + pow(-1, j + 1) * j * deltaTranslate;
			x0 = translate.x;
			
			std::unique_ptr<GameObject> redBall = std::make_unique<BallObject>(dev.Get(), devcon.Get(), XMLoadFloat4(&translate), XMVectorSet(0.1, 0.1, 0.1, 1));
			ballMesh.SetTexture(redBallTexture);
			redBall->CopyAndAddMesh(ballMesh);
			gameObjects.push_back(std::move(redBall));
		}
	}




	std::unique_ptr<GameObject> plane = std::make_unique<FloorObject>(dev.Get(), devcon.Get(), XMVectorSet(0, -1, 0, 0), XMVectorSet(15, 15, 1, 1), XMVectorSet(1, 0, 0, 0), -XM_PI / 2);
	std::shared_ptr<Texture> planeTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/pavement.jpg", 0);
	std::shared_ptr<Geometry> planeGeometry = std::make_shared<QuadGeometry<PNT>>(dev.Get());
	std::shared_ptr<Material> planeMaterial = std::make_shared<Material>(XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0), 0.0f);

	Mesh* planeMesh = new Mesh(planeGeometry, planeMaterial);
	planeMesh->SetTexture(planeTexture);
	plane->AddMesh(planeMesh);

	auto geometries = AssimpMultiModel<PNT>(dev.Get(), "Models/table.fbx").GetGeometries();

	std::unique_ptr<GameObject> monkey = std::make_unique<BallObject>(dev.Get(), devcon.Get(), XMVectorSet(0, 0.8f, 0, 0), XMVectorSet(0.8f, 0.8f, 0.8f, 1), XMVectorSet(0, 1, 0, 0), XM_PI / 2);
	std::shared_ptr<Texture> frameTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/solid_wood.png", 0);
	std::shared_ptr<Texture> tableTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/green_fabric.png", 0);
	std::shared_ptr<Texture> xingPaiStarLogo = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/xingpaistar.png", 0);
	std::shared_ptr<Texture> sideWallTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/side_wall_fabric.png", 0);
	std::shared_ptr<Texture> pocketDimmerTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/pocket_dimmer.png", 0);
	std::shared_ptr<Geometry> frameGeometry = std::move(geometries[0]);
	std::shared_ptr<Geometry> pocketDimmerGeometry = std::move(geometries[5]);
	std::shared_ptr<Geometry> sideWallGeometry = std::move(geometries[4]);
	std::shared_ptr<Geometry> tableGeometry = std::move(geometries[3]);
	std::shared_ptr<Geometry> xingPaiStarGeomRear = std::move(geometries[1]);
	std::shared_ptr<Geometry> xingPaiStarGeomFront = std::move(geometries[2]);

	std::shared_ptr<Material> dullMaterial = std::make_shared<Material>(XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0), 0.0f);

	std::unique_ptr<Mesh> frameMesh = std::make_unique<Mesh>(frameGeometry, dullMaterial);
	frameMesh->SetTexture(frameTexture);
	std::unique_ptr<Mesh> sideWallMesh = std::make_unique<Mesh>(sideWallGeometry, dullMaterial);
	sideWallMesh->SetTexture(sideWallTexture);
	std::unique_ptr<Mesh> pocketDimmerMesh = std::make_unique<Mesh>(pocketDimmerGeometry, dullMaterial);
	pocketDimmerMesh->SetTexture(pocketDimmerTexture);
	std::unique_ptr<Mesh> tableMesh = std::make_unique<Mesh>(tableGeometry, dullMaterial);
	tableMesh->SetTexture(tableTexture);
	std::unique_ptr<Mesh> xinPaiStarRearMesh = std::make_unique<Mesh>(xingPaiStarGeomRear, dullMaterial);
	xinPaiStarRearMesh->SetTexture(xingPaiStarLogo);
	std::unique_ptr<Mesh> xinPaiStarFrontMesh = std::make_unique<Mesh>(xingPaiStarGeomFront, dullMaterial);
	xinPaiStarFrontMesh->SetTexture(xingPaiStarLogo);

	monkey->AddMesh(std::move(frameMesh));
	monkey->AddMesh(std::move(sideWallMesh));
	monkey->AddMesh(std::move(pocketDimmerMesh));
	monkey->AddMesh(std::move(tableMesh));
	monkey->AddMesh(std::move(xinPaiStarRearMesh));
	monkey->AddMesh(std::move(xinPaiStarFrontMesh));
	
	//std::unique_ptr<GameObject> torus = std::make_unique<BallObject>(dev.Get(), devcon.Get(), XMVectorSet(3.0, 0.3, 0.0, 0.0), XMVectorSet(1, 1, 1, 1));
	//std::shared_ptr<Geometry> torusGeometry = std::make_shared<AssimpModel<PNT>>(dev.Get(), "Models/torus.obj");
	//std::shared_ptr<Material> torusMaterial = std::make_shared<Material>(XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0), 100.0f);
	//std::unique_ptr<Mesh> torusMesh = std::make_unique<Mesh>(torusGeometry, torusMaterial);
	//torusMesh->SetTexture(monkeyTexture);
	//torus->AddMesh(std::move(torusMesh));

	std::unique_ptr<GameObject> fullScreenQuad = std::make_unique<FullScreenQuadObject>(dev.Get(), devcon.Get(), XMVectorSet(0, 0, 0, 1), XMVectorSet(1, 1, 1, 1));
	std::shared_ptr<Geometry> fullScreenQuadGeometry = std::make_shared<FullScreenQuadGeometry<PT>>(dev.Get());
	std::unique_ptr<Mesh> fullScreenMesh = std::make_unique<Mesh>(fullScreenQuadGeometry);
	fullScreenQuad->AddMesh(std::move(fullScreenMesh));
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
	//gameObjects.push_back(std::move(torus));
	gameObjects.push_back(std::move(whiteBall));
	gameObjects.push_back(std::move(yellowBall));
	gameObjects.push_back(std::move(greenBall));
	gameObjects.push_back(std::move(brownBall));
	gameObjects.push_back(std::move(blueBall));
	gameObjects.push_back(std::move(pinkBall));
	gameObjects.push_back(std::move(blackBall));

	dirLight = std::make_unique<DirectionalLight>(XMFLOAT3(12, 16, 6), XMFLOAT3(1, 1, 1));
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
	shaderProgramPhongBlinn->SetTexture(nullptr, 1);	// a render targetet kivesszük a phong blinn pipeline pixel shaderéből.
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
		// FPS korlát
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




