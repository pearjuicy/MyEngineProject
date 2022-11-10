#include "EnginePCH.h"
#include "Export/ID3DRenderer.h"
#include "Renderer/D3DRenderer.h"

D3D_RENDERER std::shared_ptr<SEARenderer::ID3DRenderer> SEARenderer::D3DRendererInstance()
{
	if (SEARenderer::GRenderer == nullptr)
		GRenderer = std::make_shared<SEARenderer::D3DRenderer>();

	return GRenderer;
}

D3D_RENDERER void SEARenderer::D3DRendererRelease()
{
	GRenderer.reset();
}
