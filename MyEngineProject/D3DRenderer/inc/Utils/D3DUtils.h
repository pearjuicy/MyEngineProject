#pragma once

#ifndef D3DUTIL_H
#define D3DTUIL_H

#include <d3d11_1.h>

// HR 에러 메세지 출력용
#if defined (DEBUG) || defined(_DEBUG)
#ifndef HR
#define HR(x)												\
	{														\
		HRESULT hr = (x);									\
		if(FAILED(hr))										\
		{													\
			LPWSTR output;									\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		\
				FORMAT_MESSAGE_IGNORE_INSERTS		 |		\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,				\
				NULL,										\
				hr,											\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
				(LPTSTR) &output,							\
				0,											\
				NULL);										\
			MessageBox(NULL, output, L"Error", MB_OK);		\
		}													\
	}

#endif
#endif

#define SAFE_RELEASE(p) { if ((p)) { (p)->Release(); (p) = nullptr;}}
#endif
