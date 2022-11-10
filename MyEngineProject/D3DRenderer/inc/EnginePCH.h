#pragma once
#define _XM_NO_INTRINSICS_
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <wrl.h>

#include <d3d11_1.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>

#include "Utils/D3DUtils.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "d3dcomplier.lib")
#pragma comment(lib, "dxguid.lib")