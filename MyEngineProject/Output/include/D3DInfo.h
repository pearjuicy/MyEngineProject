#pragma once
#include "dllDefine.h"

namespace SEARenderer
{
	struct D3D_RENDERER WindowInfo
	{
		HWND	hwnd;
		__int32 width;
		__int32 height;
		bool	windowed;
	};
}