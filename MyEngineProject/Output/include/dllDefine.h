#pragma once

#ifdef D3DRENDERER_EXPORTS
#define D3D_RENDERER __declspec(dllexport)
#define D3D_RENDERER_TEMPLATE_EXPORT
#else
#define D3D_RENDERER __declspec(dllimport)
#define D3D_RENDERER_TEMPLATE_EXPORT extern
#endif
