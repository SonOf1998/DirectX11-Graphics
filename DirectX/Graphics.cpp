#include "pch.h"
#include "Graphics.h"

// from DirectX
#include "ArrowSet.h"
#include "ArmchairSet.h"
#include "HResultException.h"
#include "BallSet.h"
#include "ChairSet.h"
#include "OverlaySet.h"
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
#include "WhiteBallObject.h"
#include "WallObject.h"
#include "FloorObject.h"
#include "Pipeline.h"
#include "PerspectiveCamera.h"
#include "Camera.h"
#include "OrthographicCamera.h"
#include "RenderTargetTexture.h"
#include "FullScreenQuadObject.h"
#include "FullScreenQuadGeometry.h"
#include "SnookerTableObject.h"
#include "CueObject.h"
#include "Renderable.h"
#include "SystemClass.h"
#include "Resources.h"

// from SnookerLogic
#include "RoundManager.h"

// from Sounds
#include "SoundManager.h"

Graphics::Graphics(UINT screenWidth, UINT screenHeight, HWND hwnd) : hwnd(hwnd)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	InitializeDirectX11();
	InitializeImGui();
	RenderInitalization();	

	// After setting up DirectX, and object using width / height
	// (or aspect ratio deduced from them)
	// we can finally evaluate the fullscreen flag
	if (FULLSCREEN)
	{
		swapchain->SetFullscreenState(true, nullptr);
	}
}

void Graphics::RenderInitalization()
{
	camera = std::make_unique<PerspectiveCamera>(XMVectorSet(0, 2, 6, 1), XMVectorSet(0, 0, 3, 1), static_cast<float>(screenWidth) / screenHeight);
	overlayCamera = std::make_unique<OrthographicCamera>();		// will get its parameters from GRAPHICS's statics
	SoundManager::GetInstance().InitializeCamera(camera.get());	// for proper 3D game sounds
	//RoundManager::Initialize();	// init players

	overlaySet = std::make_unique<OverlaySet>(dev.Get(), devcon.Get());
	RoundManager& rm = RoundManager::GetInstance();
	rm.InitOverlaySystem(dev.Get(), devcon.Get(), reinterpret_cast<OverlaySet*>(overlaySet.get()));

	std::unique_ptr<CueObject> cue = std::make_unique<CueObject>(dev.Get(), devcon.Get(), camera.get(), XMVectorSet(0, SNOOKER_TABLE_POS_Y + BALL_RADIUS * 2, 7.5f, 0), XMVectorSet(0.7f, 0.7f, 0.7f, 1), XMVectorSet(1, 0, 0, 0), 0);
	ballSet = std::make_unique<BallSet>(dev.Get(), devcon.Get(), camera.get(), cue.get());
	std::unique_ptr<GameObject> snookerTable = std::make_unique<SnookerTableObject>(dev.Get(), devcon.Get());
	armchairSet = std::make_unique<ArmchairSet>(dev.Get(), devcon.Get());
	chairSet = std::make_unique<ChairSet>(dev.Get(), devcon.Get());
	arrowSet = std::make_unique<ArrowSet>(dev.Get(), devcon.Get(), camera.get(), reinterpret_cast<BallSet*>(ballSet.get()));

	std::unique_ptr<GameObject> plane = std::make_unique<FloorObject>(dev.Get(), devcon.Get(), XMVectorSet(0, -1, 0, 0), XMVectorSet(35, 40, 1, 1), XMVectorSet(1, 0, 0, 0), -XM_PI / 2);
	std::shared_ptr<Texture> planeTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/pavement.jpg", 0);
	std::shared_ptr<Geometry> planeGeometry = std::make_shared<QuadGeometry<PNT>>(dev.Get(), 30);
	std::shared_ptr<Material> planeMaterial = std::make_shared<Material>(XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0, 0, 0), 1.0f);
	Mesh* planeMesh = new Mesh(planeGeometry, planeMaterial);
	planeMesh->SetTexture(planeTexture);
	plane->AddMesh(planeMesh);
	   

	std::shared_ptr<Texture> wallTexture = std::make_shared<Texture>(dev.Get(), devcon.Get(), L"Textures/wall.jpg", 0);
	std::shared_ptr<Geometry> wallGeometry = std::make_shared<QuadGeometry<PNT>>(dev.Get(), 1.35f);
	std::shared_ptr<Material> wallMaterial = std::make_shared<Material>(XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1, 1, 1), 1.0f);
	Mesh wallMesh(wallGeometry, wallMaterial);
	wallMesh.SetTexture(wallTexture);

	std::unique_ptr<GameObject> wall1 = std::make_unique<WallObject>(dev.Get(), devcon.Get(), XMVectorSet(0, 0, -20, 0), XMVectorSet(50, 25, 0, 0));
	std::unique_ptr<GameObject> wall2 = std::make_unique<WallObject>(dev.Get(), devcon.Get(), XMVectorSet(0, 0, 20, 0), XMVectorSet(50, 25, 0, 0), XMVectorSet(0, 1, 0, 0), XM_PI);
	std::unique_ptr<GameObject> wall3 = std::make_unique<WallObject>(dev.Get(), devcon.Get(), XMVectorSet(-20, 0, 0, 0), XMVectorSet(50, 25, 0, 0), XMVectorSet(0, 1, 0, 0), XM_PI / 2);
	std::unique_ptr<GameObject> wall4 = std::make_unique<WallObject>(dev.Get(), devcon.Get(), XMVectorSet(20, 0, 0, 0), XMVectorSet(50, 25, 0, 0), XMVectorSet(0, 1, 0, 0), 3 * XM_PI / 2);
	std::unique_ptr<GameObject> wall5 = std::make_unique<WallObject>(dev.Get(), devcon.Get(), XMVectorSet(0, 13, 0, 0), XMVectorSet(50, 50, 0, 0), XMVectorSet(1, 0, 0, 0), XM_PI / 2);


	wall1->CopyAndAddMesh(wallMesh);
	wall2->CopyAndAddMesh(wallMesh);
	wall3->CopyAndAddMesh(wallMesh);
	wall4->CopyAndAddMesh(wallMesh);
	wall5->CopyAndAddMesh(wallMesh);
	

	std::unique_ptr<GameObject> fullScreenQuad = std::make_unique<FullScreenQuadObject>(dev.Get(), devcon.Get(), XMVectorSet(0, 0, 0, 1), XMVectorSet(1, 1, 1, 1));
	std::shared_ptr<Geometry> fullScreenQuadGeometry = std::make_shared<FullScreenQuadGeometry<PT>>(dev.Get());
	std::unique_ptr<Mesh> fullScreenMesh = std::make_unique<Mesh>(fullScreenQuadGeometry);
	fullScreenQuad->AddMesh(std::move(fullScreenMesh));
	mirror = std::move(fullScreenQuad);


	environmentParts.push_back(std::move(plane));
	environmentParts.push_back(std::move(wall1));
	environmentParts.push_back(std::move(wall2));
	environmentParts.push_back(std::move(wall3));
	environmentParts.push_back(std::move(wall4));
	environmentParts.push_back(std::move(wall5));
	gameObjects.push_back(std::move(snookerTable));
	gameObjects.push_back(std::move(cue));

	dirLight = std::make_unique<DirectionalLight>(XMFLOAT3(0, 1, 0), XMFLOAT3(1, 0, 1));
	shadowMap.reset(new RenderTargetTexture(dev.Get(), screenWidth, screenHeight, DXGI_FORMAT_R32_FLOAT));

	pipelinePhongBlinn.reset(Pipeline::Create<InputLayoutPNT>(dev.Get(), devcon.Get(), L"Shaders/Phong-Blinn/bin/vs.cso", L"Shaders/Phong-Blinn/bin/ps.cso"));
	pipelineMirror.reset(Pipeline::Create<InputLayoutPT>(dev.Get(), devcon.Get(), L"Shaders/FullScreenQuadPT/bin/vs.cso", L"Shaders/FullScreenQuadPT/bin/ps.cso"));
	pipelineMirror->SetSampler(FILTERING::NEAREST, 0);
	pipelineShadowMap.reset(Pipeline::Create<InputLayoutP>(dev.Get(), devcon.Get(), SHADOWMAP_DIRECTIONAL_VS, SHADOWMAP_DIRECTIONAL_PS));
	pipelineLoDTess.reset(Pipeline::Create<InputLayoutP>(dev.Get(), devcon.Get(), BALL_HWTESS_LOD_VS, BALL_HWTESS_LOD_PS, nullptr, BALL_HWTESS_LOD_HS, BALL_HWTESS_LOD_DS));
	pipelineBezierQuad.reset(Pipeline::Create<InputLayoutPNT>(dev.Get(), devcon.Get(), BEZIER_QUAD_VS, BEZIER_QUAD_PS, nullptr, BEZIER_QUAD_HS, BEZIER_QUAD_DS));
	pipelineOverlay.reset(Pipeline::Create<InputLayoutPT>(dev.Get(), devcon.Get(), OVERLAY_VS, OVERLAY_PS));
	pipelineBillboard.reset(Pipeline::Create<InputLayoutP>(dev.Get(), devcon.Get(), BILLBOARD_VS, BILLBOARD_PS, BILLBOARD_GS));
}


// helper function for RenderFrame


// Handles click event from "Quality: XXX" button
static void StepQuality(std::string& qualityStr)
{
	switch (Pipeline::mipOffset)
	{
	case TEXTURE_QUALITY_OFFSET::HIGH:
		Pipeline::mipOffset = TEXTURE_QUALITY_OFFSET::MEDIUM;
		BallObject::maxTessFactor = 24;
		qualityStr = QUALITY_BTN_MEDIUM_STR;
		break;
	case TEXTURE_QUALITY_OFFSET::MEDIUM:
		Pipeline::mipOffset = TEXTURE_QUALITY_OFFSET::LOW;
		BallObject::maxTessFactor = 16;
		qualityStr = QUALITY_BTN_LOW_STR;
		break;
	case TEXTURE_QUALITY_OFFSET::LOW:
		Pipeline::mipOffset = TEXTURE_QUALITY_OFFSET::VERY_LOW;
		BallObject::maxTessFactor = 8;
		qualityStr = QUALITY_BTN_VLOW_STR;
		break;
	case TEXTURE_QUALITY_OFFSET::VERY_LOW:
		Pipeline::mipOffset = TEXTURE_QUALITY_OFFSET::HIGH;
		BallObject::maxTessFactor = 32;
		qualityStr = QUALITY_BTN_HIGH_STR;
		break;
	default:
		throw std::runtime_error("Unknown offset");
		break;
	}

}

void Graphics::RenderFrame(float t, float dt)
{	
	camera->Animate(t, dt);


	// Rendering to texture //
	//RenderTargetTexture rtt(dev.Get(), screenWidth / 10, screenHeight / 10, DXGI_FORMAT_R32G32B32A32_FLOAT);
	pipelinePhongBlinn->SetTexture(nullptr, 1);	// a render targetet kivesszük a phong blinn pipeline pixel shaderébõl.
	SetRenderTargetToTexture(*shadowMap);
	devcon->ClearRenderTargetView(shadowMap->GetRenderTargetView(), COLOR::SM_WHITE);
	devcon->ClearDepthStencilView(shadowMap->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);


	for (auto& gameObject : gameObjects)
	{
		gameObject->Animate(t, dt);
		gameObject->RenderToShadowMap(devcon.Get(), pipelineShadowMap.get(), dirLight.get());
	}
	ballSet->Animate(t, dt);
	ballSet->RenderToShadowMap(devcon.Get(), pipelineShadowMap.get(), dirLight.get());
	chairSet->RenderToShadowMap(devcon.Get(), pipelineShadowMap.get(), dirLight.get());

	// Rendering to backbuffer //
	SetRenderTargetToBackBuffer();
	devcon->ClearRenderTargetView(renderTargetView.Get(), COLOR::GRAY);
	devcon->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	pipelinePhongBlinn->Use();
	pipelinePhongBlinn->SetTexture(shadowMap->GetShaderResourceView(), 1);
	for (auto& gameObject : gameObjects)
	{
		gameObject->Animate(t, dt);
		gameObject->Render(devcon.Get(), pipelinePhongBlinn.get(), camera.get(), dirLight.get());
	}
	for (auto& part : environmentParts)
	{
		part->Animate(t, dt);
		part->Render(devcon.Get(), pipelinePhongBlinn.get(), camera.get(), dirLight.get());
	}
	
	pipelineBezierQuad->Use();
	armchairSet->Render(devcon.Get(), pipelineBezierQuad.get(), camera.get());
	chairSet->Render(devcon.Get(), pipelineBezierQuad.get(), camera.get());

	pipelineLoDTess->Use();
	ballSet->Animate(t, dt);
	ballSet->Render(devcon.Get(), pipelineLoDTess.get(), camera.get());

	devcon->OMSetBlendState(overlayBlendState.Get(), nullptr, 0xFFFFFF);
	pipelineBillboard->Use();
	arrowSet->Animate(t, dt);
	arrowSet->Render(devcon.Get(), pipelineBillboard.get(), camera.get());

	pipelineOverlay->Use();
	overlaySet->Animate(t, dt);
	overlaySet->Render(devcon.Get(), pipelineOverlay.get(), overlayCamera.get());
	devcon->OMSetBlendState(defaultBlendState.Get(), nullptr, 0xFFFFFF);



	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	RoundManager& rm = RoundManager::GetInstance();

	ImGui::NewFrame();	
	ImGui::SetNextWindowPos(ImVec2(static_cast<float>(screenWidth - 135), static_cast<float>(screenHeight - 380)));
	ImGui::SetNextWindowSize(ImVec2(110, 125));
	ImGui::Begin("Cue ball", nullptr, ImGuiWindowFlags_NoResize);
	
	if (rm.IsRoundGoing())
	{
		const bool falseLiteral = false;
		ImGui::Checkbox("Aim", &falseLiteral);
		ImGui::Checkbox("Fine aim", &falseLiteral);
		ImGui::Checkbox("Spin", &falseLiteral);
		ImGui::Checkbox("Shoot", &falseLiteral);
	}
	else
	{
		bool b = false;
		b |= ImGui::Checkbox("Aim", &(WhiteBallObject::isInAimMode));
		b |= ImGui::Checkbox("Fine aim", &(WhiteBallObject::isInFineAimMode));
		b |= ImGui::Checkbox("Spin", &(WhiteBallObject::isInSpinMode));
		b |= ImGui::Checkbox("Shoot", &(WhiteBallObject::isInShootMode));
		if (b)
			WhiteBallObject::SwitchModes();
	}	
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(static_cast<float>(screenWidth - 140), static_cast<float>(screenHeight - 240)));
	ImGui::SetNextWindowSize(ImVec2(120, 120));
	ImGui::Begin("Control", nullptr, ImGuiWindowFlags_NoResize);
	if (ImGui::Button("Walk around", ImVec2(100, 20))) {
		if (!rm.IsRoundGoing() && !WhiteBallObject::isInSpinMode)
		{
			rm.EnterWalkMode();
			WhiteBallObject::isInAimMode = false;
			WhiteBallObject::isInFineAimMode = false;
			WhiteBallObject::isInShootMode = false;
			WhiteBallObject::isInSpinMode = false;
		}
	}
	if (ImGui::Button("Nominate", ImVec2(100, 20))) {
		if (rm.CanNominate())
		{
			rm.EnterNominateMode();
			SoundManager& sm = SoundManager::GetInstance();
			XMFLOAT3 whiteBallPos;
			XMStoreFloat3(&whiteBallPos, reinterpret_cast<BallSet*>(ballSet.get())->GetWhiteBallPosition());
			sm.PlaySound(NOMINATION_SOUND, whiteBallPos, nullptr, nullptr);
		}
	}
	ImGui::NewLine();
	if (ImGui::Button("Concede frame", ImVec2(100, 20)))
	{
		if (!rm.IsRoundGoing())
		{
			reinterpret_cast<BallSet*>(ballSet.get())->InitTable();
			rm.DeclareWinner(true);
		}
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(static_cast<float>(screenWidth - 170), static_cast<float>(screenHeight - 100)));
	ImGui::SetNextWindowSize(ImVec2(160, 80));
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoResize);

	static std::string qualityStr = QUALITY_BTN_HIGH_STR;
	if (ImGui::Button(qualityStr.c_str(), ImVec2(140, 20))) {
		StepQuality(qualityStr);
	}
	static std::string soundStr = SOUNDS_BTN_ON_STR;
	if (ImGui::Button(soundStr.c_str(), ImVec2(140, 20))) {
		if (SoundManager::muted)
			soundStr = SOUNDS_BTN_ON_STR;
		else
			soundStr = SOUNDS_BTN_OFF_STR;

		SoundManager::muted = !SoundManager::muted;
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(70, 20));
	ImGui::SetNextWindowBgAlpha(0.2f);
	ImGui::Begin("Target", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::Text("Target:");
	ImGui::End();


	ImGui::SetNextWindowPos(ImVec2(10, static_cast<float>(screenHeight - 130)));
	ImGui::SetNextWindowSize(ImVec2(500, 120));
	ImGui::Begin("Scoreboard", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::Columns(7);
	ImGui::SetColumnWidth(0, 70);
	ImGui::SetColumnWidth(1, 60);
	ImGui::SetColumnWidth(2, 115);
	ImGui::SetColumnWidth(3, 5);
	ImGui::SetColumnWidth(4, 70);
	ImGui::SetColumnWidth(5, 60);
	ImGui::Text(rm.GetPlayerDisplayName(PL1).c_str());
	ImGui::NextColumn();
	ImGui::Text("Break");
	ImGui::NewLine();
	ImGui::Text("Points");
	ImGui::NewLine();
	ImGui::Text("Frames");
	ImGui::NextColumn();
	ImGui::Text(std::to_string(rm.GetPlayer(PL1)->GetBreak()).c_str());
	ImGui::NewLine();
	ImGui::Text(std::to_string(rm.GetPlayer(PL1)->GetPoints()).c_str());
	ImGui::NewLine();
	ImGui::Text(std::to_string(rm.GetPlayer(PL1)->GetWonFrame()).c_str());
	ImGui::NextColumn();
	ImGui::NextColumn();
	ImGui::Text(rm.GetPlayerDisplayName(PL2).c_str());
	ImGui::NextColumn();
	ImGui::Text("Break");
	ImGui::NewLine();
	ImGui::Text("Points");
	ImGui::NewLine();
	ImGui::Text("Frames");
	ImGui::NextColumn();
	ImGui::Text(std::to_string(rm.GetPlayer(PL2)->GetBreak()).c_str());
	ImGui::NewLine();
	ImGui::Text(std::to_string(rm.GetPlayer(PL2)->GetPoints()).c_str());
	ImGui::NewLine();
	ImGui::Text(std::to_string(rm.GetPlayer(PL2)->GetWonFrame()).c_str());
	ImGui::End();

	if (rm.IsWhiteDroppedLastRound() && rm.IsWhitePlaced() && (WhiteBallObject::isInAimMode || WhiteBallObject::isInFineAimMode))
	{
		ImGuiStyle & style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Button] = ImVec4(0.5f, 0.5f, 0, 0.8f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.55f, 0.55f, 0, 0.8f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.6f, 0.6f, 0, 0.8f);
		ImGui::SetNextWindowPos(ImVec2(30, static_cast<float>(screenHeight - 190)));
		ImGui::SetNextWindowSize(ImVec2(120, 40));
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::Begin("Move", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		if (ImGui::Button("Move cue ball", ImVec2(105, 25)))
		{
			rm.SetWhitePlaced(false);
		}
		ImGui::End();


		style.Colors[ImGuiCol_Button] = ImVec4(0.28f, 0.24f, 0.545f, 0.8f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.29f, 0.69f, 0.8f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.416f, 0.35f, 0.8f, 0.8f);
	}	


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (VSYNC_ENABLED)
	{
		swapchain->Present(1, 0);
	}
	else
	{
		swapchain->Present(0, 0);
	}
}


Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Graphics::CreateSwapChain(IDXGIFactory1* factory)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

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
	swapChainDesc.Windowed = true;	
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	HRESULT result;
	THROW_IF_HRESULT_FAILED(factory->CreateSwapChain(dev.Get(), &swapChainDesc, swapchain.GetAddressOf()));
	
	// Disabling ALT ENTER fullscreen mode only possible
	// throught the IDXGIFactory returned by the GetParent
	// function of the IDXGISwapChain
	
	ComPtr<IDXGIFactory1> factoryForDisablingALTENTER;
	swapchain->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(factoryForDisablingALTENTER.GetAddressOf()));
	factoryForDisablingALTENTER->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
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

void Graphics::CreateBlendStates()
{
	D3D11_BLEND_DESC defBlendStateDesc = {};
	defBlendStateDesc.RenderTarget[0].BlendEnable = false;
	defBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	dev->CreateBlendState(&defBlendStateDesc, defaultBlendState.GetAddressOf());

	D3D11_BLEND_DESC overlayBlendStateDesc = {};
	overlayBlendStateDesc.RenderTarget[0].BlendEnable = true;
	overlayBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	overlayBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	overlayBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	overlayBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	overlayBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	overlayBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	overlayBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	dev->CreateBlendState(&overlayBlendStateDesc, overlayBlendState.GetAddressOf());

	devcon->OMSetBlendState(defaultBlendState.Get(), nullptr, 0xFFFFFF);
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
	CreateBlendStates();


	viewport.Width = static_cast<float>(screenWidth);
	viewport.Height = static_cast<float>(screenHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	devcon->RSSetViewports(1, &viewport);
}

void Graphics::InitializeImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(screenWidth), static_cast<float>(screenHeight));
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(dev.Get(), devcon.Get());
	ImGui::StyleColorsClassic();
	

	ImGuiStyle & style = ImGui::GetStyle();
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.184f, 0.607f, 0.193f, 1.00f);
	// purple buttons
	style.Colors[ImGuiCol_Button] = ImVec4(0.28f, 0.24f, 0.545f, 0.8f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.29f, 0.69f, 0.8f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.416f, 0.35f, 0.8f, 0.8f);

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
	size_t maxVideoMemory = 0;

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

void Graphics::SwitchMode(bool toFullscreen)
{
	if (toFullscreen)
	{ 
		int screenWidth;
		int screenHeight;
		SystemClass::GetScreenResolution(screenWidth, screenHeight);

		DXGI_MODE_DESC modeDesc = {};
		modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		modeDesc.RefreshRate.Numerator = 60;
		modeDesc.RefreshRate.Denominator = 1;
		modeDesc.Width = screenWidth;
		modeDesc.Height = screenHeight;

		swapchain->ResizeTarget(&modeDesc);
		swapchain->SetFullscreenState(true, nullptr);
	}
	else
	{
		swapchain->SetFullscreenState(false, nullptr);
	}
}

void Graphics::Resize(UINT newWidth, UINT newHeight)
{
	HRESULT result;

	float magnificationRatio = std::min(static_cast<float>(newWidth) / 1280, static_cast<float>(newHeight) / 720);
	
	screenWidth = newWidth;
	screenHeight = newHeight;
	camera->SetAspectRatio(static_cast<float>(screenWidth) / screenHeight);

	devcon->ClearState();
	devcon->OMSetRenderTargets(0, nullptr, nullptr);
	renderTargetView = nullptr;

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

	reinterpret_cast<OverlaySet*>(overlaySet.get())->MarkForUpdate();

	devcon->RSSetViewports(1, &viewport);	
	ImGuiIO& io = ImGui::GetIO();
	//io.FontGlobalScale = magnificationRatio;
	io.DisplaySize = ImVec2(static_cast<float>(screenWidth), static_cast<float>(screenHeight));
}

void Graphics::ForwardWheelMessage(short deltaZ)
{
	static constexpr short wheelUnit = 120; // Microsoft made const
	camera->MagnifyAimMode(deltaZ / wheelUnit);
}




