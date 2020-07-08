#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _ENABLE_EXTENDED_ALIGNED_STORAGE

// standard library //
#include <cmath>
#include <cstring>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

// WINAPI //
#include <comdef.h>
#include <Windows.h>
#include <wrl/client.h>
#include <stringapiset.h>	

// DirectX //
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <DirectXMath.h>

// DirectXTK //
#include <WICTextureLoader.h>

// XAudio2 //
#include <x3daudio.h>
#include <xapo.h>
#include <hrtfapoapi.h>
#include <xapobase.h>
#include <xapofx.h>
#include <xaudio2.h>
#include <xaudio2fx.h>

// Assimp //
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>	

// imgui //
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

// libs //
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxguid.lib")

// own //
#include "HResultException.h"
#include "InputClass.h"
#include "Logger.h"
#include "SimpleDXMath.h"


using namespace DirectX;
using namespace Microsoft::WRL;

using uint = unsigned int;

inline LPCWSTR WindowTitle = L"Snooker DirectX11 - BME VIK Szakdolgozat";
constexpr UINT MaxInstanceCount = 50;

constexpr UINT	MSAALevel = 4;
constexpr bool	FULLSCREEN = false;
constexpr bool	VSYNC_ENABLED = false;