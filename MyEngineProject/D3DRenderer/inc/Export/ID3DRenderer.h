#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include "dllDefine.h"
#include "D3DInfo.h"

namespace SEARenderer 
{
	class D3D_RENDERER ID3DRenderer
	{
	protected:
		ID3DRenderer() = default;
		virtual ~ID3DRenderer() = default;

	public:
		virtual bool Initialize(WindowInfo& info) abstract;
		virtual bool Shutdown() abstract;
		virtual bool Update() abstract;
		virtual bool Render() abstract;

		virtual WindowInfo& GetWindow() abstract;

		virtual bool BeginScene(float red, float green, float blue, float alpha) abstract;
		virtual bool EndScene() abstract;

		virtual bool GetDevice(Microsoft::WRL::ComPtr<ID3D11Device>& output) abstract;
		virtual bool GetDeviceContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& output) abstract;

		virtual bool GetVideoCardInfo(char* cardName, int& output) abstract;

		virtual bool OnResize() abstract;
	};

	static std::shared_ptr<ID3DRenderer> GRenderer = nullptr;

	D3D_RENDERER std::shared_ptr<ID3DRenderer> D3DRendererInstance();
	D3D_RENDERER void D3DRendererRelease();
}