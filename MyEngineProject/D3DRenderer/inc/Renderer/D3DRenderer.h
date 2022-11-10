#pragma once
#include "Export/D3DInfo.h"
#include "Export/ID3DRenderer.h"

namespace SEARenderer
{
	class D3DRenderer : public ID3DRenderer
	{
	public:
		bool Initialize(WindowInfo& info) override;
		bool Shutdown() override;
		bool Update() override;
		bool Render() override;

		WindowInfo& GetWindow() override { return m_window; }

		bool BeginScene(float red, float green, float blue, float alpha) override;
		bool EndScene() override;

		//Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
		//Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext();
		bool GetDevice(Microsoft::WRL::ComPtr<ID3D11Device>& output) override;
		bool GetDeviceContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& output) override;

		bool GetVideoCardInfo(char* cardName, int& output) override;

		bool OnResize() override;

	private:
		WindowInfo	m_window;

		bool m_vsyncEnable;
		bool m_enable4xMSAA;
		int m_videoCardMemory;
		char m_videoCardDescription[128];

		// Direct3D 11.0
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
		// Direct3D 11.1
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain_1;
		Microsoft::WRL::ComPtr<ID3D11Device1> m_device_1;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_deviceContext_1;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterState;

		D3D11_VIEWPORT m_screenViewPort;

		unsigned int m_4xMASSQuality;
	};
}