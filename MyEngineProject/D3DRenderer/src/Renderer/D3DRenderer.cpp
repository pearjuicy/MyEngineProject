#include "EnginePCH.h"
#include "Renderer/D3DRenderer.h"

namespace SEARenderer
{
	bool D3DRenderer::Initialize(WindowInfo& info)
	{
		m_window = info;

		m_swapChain = nullptr;
		m_device = nullptr;
		m_deviceContext = nullptr;

		m_swapChain_1 = nullptr;
		m_device_1 = nullptr;
		m_deviceContext_1 = nullptr;

		m_renderTargetView = nullptr;
		m_depthStencilBuffer = nullptr;
		m_depthStencilState = nullptr;
		m_depthStencilView = nullptr;
		m_rasterState = nullptr;

		m_enable4xMSAA = false;

		ZeroMemory(&m_screenViewPort, sizeof(D3D11_VIEWPORT));

		HRESULT _hr = S_OK;

		UINT _createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
		_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE _driverTypeArray[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT _numDriverTypes = ARRAYSIZE(_driverTypeArray);
		D3D_DRIVER_TYPE _driverType;

		D3D_FEATURE_LEVEL _featureLevelArray[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};
		UINT _numFeatureLevels = ARRAYSIZE(_featureLevelArray);
		D3D_FEATURE_LEVEL _featureLevel;

		// 가능한 버전 찾기
		for (UINT i = 0; i < _numDriverTypes; i++)
		{
			_driverType = _driverTypeArray[i];

			_hr = D3D11CreateDevice(nullptr, _driverType,
				nullptr, _createDeviceFlags, _featureLevelArray, _numFeatureLevels, D3D11_SDK_VERSION,
				m_device.GetAddressOf(), &_featureLevel, m_deviceContext.GetAddressOf());

			if (_hr == E_INVALIDARG)
			{
				_hr = D3D11CreateDevice(nullptr, _driverType,
					nullptr, _createDeviceFlags, &_featureLevelArray[1], _numFeatureLevels - 1, D3D11_SDK_VERSION,
					m_device.GetAddressOf(), &_featureLevel, m_deviceContext.GetAddressOf());
			}

			if (SUCCEEDED(_hr))
				break;
		}

		if (FAILED(_hr))
		{
			MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
			return false;
		}

		if ((_featureLevel != D3D_FEATURE_LEVEL_11_0) && (_featureLevel != D3D_FEATURE_LEVEL_11_1))
		{
			MessageBox(0, L"Direct3D Feature Level 11 Unsupported.", 0, 0);
			return false;
		}

		// 4X MSAA 체크
		HR(m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMASSQuality));
		assert(m_4xMASSQuality > 0);

		Microsoft::WRL::ComPtr<IDXGIDevice> _dxgiDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> _dxgiAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory1> _dxgiFactory1 = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory2> _dxgiFactory2 = nullptr;

		HR(m_device.As(&_dxgiDevice));
		HR(_dxgiDevice->GetAdapter(_dxgiAdapter.GetAddressOf()));
		HR(_dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(_dxgiFactory1.GetAddressOf())));

		_hr = _dxgiFactory1.As(&_dxgiFactory2);

		// 11.1 version
		if (_dxgiFactory2 != nullptr)
		{
			HR(m_device.As((&m_device_1)));
			HR(m_deviceContext.As(&m_deviceContext_1));

			DXGI_SWAP_CHAIN_DESC1 _swapChainDesc;
			ZeroMemory(&_swapChainDesc, sizeof(_swapChainDesc));
			_swapChainDesc.BufferCount = 1;
			_swapChainDesc.Width = m_window.width;
			_swapChainDesc.Height = m_window.height;
			_swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

			if (m_enable4xMSAA)
			{
				_swapChainDesc.SampleDesc.Count = 4;
				_swapChainDesc.SampleDesc.Quality = m_4xMASSQuality - 1;
			}

			else
			{
				_swapChainDesc.SampleDesc.Count = 1;
				_swapChainDesc.SampleDesc.Quality = 0;
			}

			_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			_swapChainDesc.Flags = 0;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC _fullScreenDesc;
			_fullScreenDesc.RefreshRate.Numerator = 60;
			_fullScreenDesc.RefreshRate.Denominator = 1;
			_fullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			_fullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			_fullScreenDesc.Windowed = TRUE;

			HR(_dxgiFactory2->CreateSwapChainForHwnd(m_device.Get(), info.hwnd,
				&_swapChainDesc, &_fullScreenDesc, nullptr, m_swapChain_1.GetAddressOf()));
			HR(m_swapChain_1.As(&m_swapChain));
		}

		// 11.0 version
		else
		{
			DXGI_SWAP_CHAIN_DESC _swapChainDesc;
			ZeroMemory(&_swapChainDesc, sizeof(_swapChainDesc));
			_swapChainDesc.BufferCount = 1;
			_swapChainDesc.BufferDesc.Width = info.width;
			_swapChainDesc.BufferDesc.Height = info.height;
			_swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			_swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			if (m_enable4xMSAA)
			{
				_swapChainDesc.SampleDesc.Count = 4;
				_swapChainDesc.SampleDesc.Quality = m_4xMASSQuality - 1;
			}

			else
			{
				_swapChainDesc.SampleDesc.Count = 1;
				_swapChainDesc.SampleDesc.Quality = 0;
			}

			_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			_swapChainDesc.OutputWindow = info.hwnd;
			_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			_swapChainDesc.Flags = 0;
			_swapChainDesc.Windowed = TRUE;

			HR(_dxgiFactory1->CreateSwapChain(m_device.Get(), &_swapChainDesc, m_swapChain.GetAddressOf()));
		}

		// ALT + Enter 막기
		_dxgiFactory1->MakeWindowAssociation(info.hwnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);

		OnResize();

		// 레거시
		/*	IDXGIDevice* _dxgiDevice = 0;
			m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&_dxgiDevice);

			IDXGIFactory* _dxgiFactory = 0;
			_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&_dxgiFactory);

			_dxgiFactory->CreateSwapChain(m_device.Get(), &_swapChainDesc, m_swapChain.GetAddressOf());

			_dxgiDevice->Release();
			_dxgiAdapter->Release();
			_dxgiFactory->Release();

			m_swapChain->ResizeBuffers(1, m_window.width, m_window.height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
			ID3D11Texture2D* _backBuffer;
			m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&_backBuffer));
			m_device->CreateRenderTargetView(_backBuffer, 0, m_renderTargetView.GetAddressOf());
			_backBuffer->Release();

			D3D11_TEXTURE2D_DESC _depthStencilDesc;
			_depthStencilDesc.Width = m_window.width;
			_depthStencilDesc.Height = m_window.height;
			_depthStencilDesc.MipLevels = 1;
			_depthStencilDesc.ArraySize = 1;
			_depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

			if (m_enable4xMSAA)
			{
				_depthStencilDesc.SampleDesc.Count = 4;
				_depthStencilDesc.SampleDesc.Quality = m_4xMASSQuality - 1;
			}

			else
			{
				_depthStencilDesc.SampleDesc.Count = 1;
				_depthStencilDesc.SampleDesc.Quality = 0;
			}

			_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
			_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			_depthStencilDesc.CPUAccessFlags = 0;
			_depthStencilDesc.MiscFlags = 0;

			D3D11_DEPTH_STENCIL_VIEW_DESC _depthStencilViewDesc;
			ZeroMemory(&_depthStencilViewDesc, sizeof(_depthStencilViewDesc));

			_depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			_depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			_depthStencilViewDesc.Texture2D.MipSlice = 0;

			m_device->CreateTexture2D(&_depthStencilDesc, 0, m_depthStencilBuffer.GetAddressOf());
			m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, m_depthStencilView.GetAddressOf());

			m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

			m_screenViewPort.TopLeftX = 0.f;
			m_screenViewPort.TopLeftY = 0.f;
			m_screenViewPort.Width = static_cast<float>(m_window.width);
			m_screenViewPort.Height = static_cast<float>(m_window.height);
			m_screenViewPort.MinDepth = 0.0f;
			m_screenViewPort.MaxDepth = 1.0f;

			m_deviceContext->RSSetViewports(1, &m_screenViewPort);*/

		return true;
	}

	bool D3DRenderer::Shutdown()
	{
		if (m_swapChain)
			m_swapChain->SetFullscreenState(false, NULL);

		return true;
	}

	bool D3DRenderer::Update()
	{
		bool _result;

		//INPUT->Update();
		//TIMER->Update();

		return true;
	}

	bool D3DRenderer::Render()
	{
		BeginScene(0.f, 1.f, 0.f, 1.f);

		// 렌더할거 추가

		EndScene();

		return true;
	}

	bool D3DRenderer::BeginScene(float red, float green, float blue, float alpha)
	{
		assert(m_deviceContext);
		assert(m_swapChain);

		float _color[4] = { red, green, blue, alpha };
		m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), _color);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		return true;
	}

	bool D3DRenderer::EndScene()
	{
		if (m_vsyncEnable == true)
		{
			HR(m_swapChain->Present(1, 0));
		}

		else
		{
			HR(m_swapChain->Present(0, 0));
		}

		return true;
	}

	//Microsoft::WRL::ComPtr<ID3D11Device> D3DRenderer::GetDevice()
	//{
	//	return m_device;
	//}

	bool D3DRenderer::GetDevice(Microsoft::WRL::ComPtr<ID3D11Device>& output)
	{
		output = m_device;
		return true;
	}

	bool D3DRenderer::GetDeviceContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& output)
	{
		output = m_deviceContext;
		return true;
	}

	//Microsoft::WRL::ComPtr<ID3D11DeviceContext> D3DRenderer::GetDeviceContext()
	//{
	//	return m_deviceContext;
	//}

	bool D3DRenderer::GetVideoCardInfo(char* cardName, int& output)
	{
		strcpy_s(cardName, 128, m_videoCardDescription);
		output = m_videoCardMemory;
		return true;
	}

	bool D3DRenderer::OnResize()
	{
		assert(m_deviceContext);
		assert(m_device);
		assert(m_swapChain);

		if (m_device != nullptr)
		{
			assert(m_deviceContext);
			assert(m_device);
			assert(m_swapChain);
		}
		m_renderTargetView.Reset();;
		m_depthStencilView.Reset();
		m_depthStencilBuffer.Reset();

		Microsoft::WRL::ComPtr<ID3D11Texture2D> _backBuffer;
		HR(m_swapChain->ResizeBuffers(1, m_window.width, m_window.height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
		HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(_backBuffer.GetAddressOf())));
		HR(m_device->CreateRenderTargetView(_backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf()));

		_backBuffer.Reset();

		D3D11_TEXTURE2D_DESC _depthStencilDesc;

		_depthStencilDesc.Width = m_window.width;
		_depthStencilDesc.Height = m_window.height;
		_depthStencilDesc.MipLevels = 1;
		_depthStencilDesc.ArraySize = 1;
		_depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		if (m_enable4xMSAA)
		{
			_depthStencilDesc.SampleDesc.Count = 4;
			_depthStencilDesc.SampleDesc.Quality = m_4xMASSQuality - 1;
		}

		else
		{
			_depthStencilDesc.SampleDesc.Count = 1;
			_depthStencilDesc.SampleDesc.Quality = 0;
		}

		_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		_depthStencilDesc.CPUAccessFlags = 0;
		_depthStencilDesc.MiscFlags = 0;

		HR(m_device->CreateTexture2D(&_depthStencilDesc, nullptr, m_depthStencilBuffer.GetAddressOf()));
		HR(m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr, m_depthStencilView.GetAddressOf()));

		m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

		m_screenViewPort.TopLeftX = 0;
		m_screenViewPort.TopLeftY = 0;
		m_screenViewPort.Width = static_cast<float>(m_window.width);
		m_screenViewPort.Height = static_cast<float>(m_window.height);
		m_screenViewPort.MaxDepth = 1.f;
		m_screenViewPort.MinDepth = 0.f;

		m_deviceContext->RSSetViewports(1, &m_screenViewPort);

		// 레거시
		//m_swapChain->ResizeBuffers(1, m_window.width, m_window.height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		//ID3D11Texture2D* _backBuffer;
		//m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&_backBuffer));
		//m_device->CreateRenderTargetView(_backBuffer, 0, m_renderTargetView.GetAddressOf());
		//_backBuffer->Release();

		//D3D11_TEXTURE2D_DESC _depthStencilDesc;
		//_depthStencilDesc.Width = m_window.width;
		//_depthStencilDesc.Height = m_window.height;
		//_depthStencilDesc.MipLevels = 1;
		//_depthStencilDesc.ArraySize = 1;
		//_depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		//if (m_enable4xMSAA)
		//{
		//	_depthStencilDesc.SampleDesc.Count = 4;
		//	_depthStencilDesc.SampleDesc.Quality = m_4xMASSQuality - 1;
		//}

		//else
		//{
		//	_depthStencilDesc.SampleDesc.Count = 1;
		//	_depthStencilDesc.SampleDesc.Quality = 0;
		//}

		//_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		//_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		//_depthStencilDesc.CPUAccessFlags = 0;
		//_depthStencilDesc.MiscFlags = 0;

		//D3D11_DEPTH_STENCIL_VIEW_DESC _depthStencilViewDesc;
		//ZeroMemory(&_depthStencilViewDesc, sizeof(_depthStencilViewDesc));

		//_depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		//_depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		//_depthStencilViewDesc.Texture2D.MipSlice = 0;

		//m_device->CreateTexture2D(&_depthStencilDesc, 0, m_depthStencilBuffer.GetAddressOf());
		//m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, m_depthStencilView.GetAddressOf());

		//m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

		return true;
	}
}