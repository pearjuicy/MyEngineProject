#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "ID3DRenderer.h"

#pragma comment(lib, "D3DRenderer")

#define DECLARE_SINGLE(type)	\
private:						\
	type() {}					\
	~type() {}					\
								\
public:							\
	static type* GetInstance()	\
	{							\
		static type instance;	\
		return &instance;		\
	}							\

#define GET_SINGLE(type)	type::GetInstance()

#define INPUT				GET_SINGLE(Input)
#define TIMER				GET_SINGLE(Timer)
#define SCENE_MANAGER		GET_SINGLE(SceneManager)
#define RESOURCE_MANAGER	GET_SINGLE(ResourceManager)

#define DELTA_TIME			GET_SINGLE(Timer)->GetDeltaTime()