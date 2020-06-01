#pragma once

#define WIN32_LEAN_AND_MEAN

// standard library //
#include <cmath>
#include <cstring>
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

// Assimp //
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>	

// libs //
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

using uint = unsigned int;
