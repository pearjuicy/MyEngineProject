#include "Client/WindowManager.h"


//bool WindowManager::Initialize(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
//{
//	TCHAR szAppName[] = _T("Direct3D Graphics Demo");
//	SEARenderer::WindowInfo _info;
//	WNDCLASS wndclass;
//
//	wndclass.style = CS_HREDRAW | CS_VREDRAW;
//	wndclass.lpfnWndProc = MsgProc;
//	wndclass.cbClsExtra = 0;
//	wndclass.cbWndExtra = 0;
//	wndclass.hInstance = hInstance;
//	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
//	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//	wndclass.lpszMenuName = NULL;
//	wndclass.lpszClassName = szAppName;
//
//	RegisterClass(&wndclass);
//
//	_info.hwnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
//		100, 100, 1400, 720, NULL, NULL, hInstance, NULL);
//
//	if (!_info.hwnd) return FALSE;
//
//	RECT _rect;
//	GetClientRect(_info.hwnd, &_rect);
//	_info.height = _rect.bottom - _rect.top;
//	_info.width = _rect.right - _rect.left;
//	
//	SEARenderer::GRenderer->Initialize(_info);
//
//	ShowWindow(_info.hwnd, nCmdShow);
//	UpdateWindow(_info.hwnd);
//
//	return TRUE;
//}

bool WindowManager::Initialize()
{
	bool _result;

	m_info.width = 1024;
	m_info.height = 768;
	m_info.windowed = true;

	WNDCLASSEX _wcex;
	DEVMODE _dmScreenSettings;

	ApplicationHandle = this;

	m_hInstance = GetModuleHandle(NULL);
	m_applicationName = L"Direct3D Graphics Demo";

	_wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	_wcex.lpfnWndProc = WndProc;
	_wcex.cbClsExtra = 0;
	_wcex.cbWndExtra = 0;
	_wcex.hInstance = m_hInstance;
	_wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	_wcex.hIconSm = _wcex.hIcon;
	_wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	_wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	_wcex.lpszMenuName = NULL;
	_wcex.lpszClassName = m_applicationName;
	_wcex.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&_wcex);

	int _posX = (GetSystemMetrics(SM_CXSCREEN) - m_info.width) / 2;
	int _posY = (GetSystemMetrics(SM_CYSCREEN) - m_info.height) / 2;

	m_info.hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_OVERLAPPEDWINDOW,
		_posX, _posY,
		m_info.width, m_info.height,
		NULL, NULL, m_hInstance, NULL);

	ShowWindow(m_info.hwnd, SW_SHOW);
	SetForegroundWindow(m_info.hwnd);

	// 인풋이랑 타이머도 여기서

	SEARenderer::GRenderer = SEARenderer::D3DRendererInstance();
	_result = SEARenderer::GRenderer->Initialize(m_info);

	// 추가 할게 있다면 여기서

	if (_result == false)
	{
		MessageBox(m_info.hwnd, L"Direct3D Init Failed", L"Error", MB_OK);
		return false;
	}

	// 씬은 이 이후에

	return true;
}

bool WindowManager::Loop()
{
	MSG _msg;

	while (TRUE)
	{
		if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (_msg.message == WM_QUIT)
				break;

			DispatchMessage(&_msg);
		}

		else
		{
			// 업데이트 해야할게 있다면 렌더보다 먼저

			SEARenderer::GRenderer->Render();
			DrawFPS();
		}
	}

	return (int)_msg.wParam;
}

bool WindowManager::Shutdown()
{
	if (SEARenderer::GRenderer->Shutdown() == false)
		return false;
		
	ShutdownWindows();
	return true;
}

LRESULT CALLBACK WindowManager::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//case WM_ACTIVATE:
		//{
		//	if (LOWORD(wParam) == WA_INACTIVE)
		//	{
		//		m_GamePaused = true;
		//		//Timer::Instance.Stop();
		//	}
		//	else
		//	{
		//		m_GamePaused = false;
		//		//Timer::Instance.Start();
		//	}

		//	break;
		//}

		//case WM_SIZE:
		//{
		//	SEARenderer::GRenderer->GetWindow().width = wParam;
		//	SEARenderer::GRenderer->GetWindow().height = lParam;

		//	if (wParam == SIZE_MINIMIZED)
		//	{
		//		m_GamePaused = true;
		//		m_Minimized = true;
		//		m_Maximized = false;
		//	}
		//	else if (wParam == SIZE_MAXIMIZED)
		//	{
		//		m_GamePaused = false;
		//		m_Minimized = false;
		//		m_Maximized = true;

		//		SEARenderer::GRenderer->OnResize();
		//	}
		//	else if (wParam == SIZE_RESTORED)
		//	{
		//		if (m_Minimized)
		//		{
		//			m_GamePaused = false;
		//			m_Minimized = false;

		//			SEARenderer::GRenderer->OnResize();

		//		}
		//		else if (m_Maximized)
		//		{
		//			m_GamePaused = false;
		//			m_Maximized = false;

		//			SEARenderer::GRenderer->OnResize();

		//		}
		//		else if (m_Resizing)
		//		{
		//			// If user is dragging the resize bars, we do not resize 
		//			// the buffers here because as the user continuously 
		//			// drags the resize bars, a stream of WM_SIZE messages are
		//			// sent to the window, and it would be pointless (and slow)
		//			// to resize for each WM_SIZE message received from dragging
		//			// the resize bars.  So instead, we reset after the user is 
		//			// done resizing the window and releases the resize bars, which 
		//			// sends a WM_EXITSIZEMOVE message.
		//		}
		//		else
		//		{
		//			SEARenderer::GRenderer->OnResize();
		//		}
		//	}

		//	break;
		//}

		//case WM_ENTERSIZEMOVE:
		//	m_GamePaused = true;
		//	m_Resizing = true;
		//	//Timer::Instance.Stop();
		//	break;

		//case WM_EXITSIZEMOVE:
		//	m_GamePaused = false;
		//	m_Resizing = false;
		//	//Timer::Instance.Start();
		//	SEARenderer::GRenderer->OnResize();
		//	break;

		//	// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		//	// a key that does not correspond to any mnemonic or accelerator key. 
		//case WM_MENUCHAR:
		//	return MAKELRESULT(0, MNC_CLOSE);

		//	// 윈도우가 너무 작아지지 않도록 메시지를 잡습니다.
		//case WM_GETMINMAXINFO:
		//	((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		//	((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		//	break;

		//case WM_LBUTTONDOWN:
		//case WM_MBUTTONDOWN:
		//case WM_RBUTTONDOWN:
		//	break;

		//case WM_LBUTTONUP:
		//case WM_MBUTTONUP:
		//case WM_RBUTTONUP:
		//	break;

		//case WM_MOUSEMOVE:
		//	break;

		//case WM_DESTROY:
		//	PostQuitMessage(0);
		//	break;

		default:
			return DefWindowProc(hWnd,
				message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK WindowManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		// Check if the window is being closed. 
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		// All other messages pass to the message handler in the system class. 
		default:
		{
			return ApplicationHandle->MsgProc(hWnd, message, wParam, lParam);
		}
	}
}

void WindowManager::ShutdownWindows()
{
	ShowCursor(true);

	DestroyWindow(m_info.hwnd);
	m_info.hwnd = NULL;

	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance - NULL;

	ApplicationHandle = NULL;

	return;
}

bool WindowManager::DrawFPS()
{
	// 타이머 만들고 대입
	unsigned __int32 _fps = 0;

	WCHAR _text[100] = L"";
	::wsprintf(_text, L"FPS : %d", _fps);

	::SetWindowText(SEARenderer::GRenderer->GetWindow().hwnd, _text);

	return true;
}
