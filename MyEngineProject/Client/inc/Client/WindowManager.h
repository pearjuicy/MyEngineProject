#pragma once
#include "ClientPCH.h"
class WindowManager
{
public:
	WindowManager() = default;
	~WindowManager() = default;

private:
	HINSTANCE m_hInstance;
	SEARenderer::WindowInfo m_info;

	LPCWSTR m_applicationName;

	bool m_GamePaused;
	bool m_Minimized;
	bool m_Maximized;
	bool m_Resizing;

public:
	bool Initialize();
	bool Loop();
	bool Shutdown();

	LRESULT CALLBACK MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

private:
	void ShutdownWindows();
	bool DrawFPS();
};

static WindowManager* ApplicationHandle = 0;