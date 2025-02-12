#ifndef _CGRENDERAPI_H_
#define _CGRENDERAPI_H_

#include "CFLRenderMacros.h"
#include "CFLRender_Exports.h"
#include "CFLglm.h"

bool CFL_GLRENDER_API CFLRenderGL_Init(const TCHAR* pluginPath);
int  CFL_GLRENDER_API CFLRenderGL_CreateContext(HWND hWnd, HGLRC hGLRC, int nWidth = 0, int nHeight = 0);
bool CFL_GLRENDER_API CFLRenderGL_ReleaseContext(int Device);

bool CFL_GLRENDER_API CFLRenderGL_MakeCurrent(int Device);
bool CFL_GLRENDER_API CFLRenderGL_UnMakeCurrent(int Device);

bool CFL_GLRENDER_API CFLRenderGL_SetViewport(int Device, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
bool CFL_GLRENDER_API CFLRenderGL_SetScissor(int Device, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
bool CFL_GLRENDER_API CFLRenderGL_SetViewport(int Device, CGRECT viewPort);
bool CFL_GLRENDER_API CFLRenderGL_SetScissor(int Device, CGRECT scissor);
bool CFL_GLRENDER_API CFLRenderGL_GetViewport(int Device, CGRECT* rect);
bool CFL_GLRENDER_API CFLRenderGL_GetScissor(int Device, CGRECT* rect);

bool CFL_GLRENDER_API CFLRenderGL_RenderTest(int Device, float time);
bool CFL_GLRENDER_API CFLRenderGL_Render(int hDevice, int hVertexBuffer, int hIndexBuffer, int hVertexShader, int hGeometryShader, int hPixShader, int nStart, int nVertexCount, int nVertexBase);

glm::vec3 CFL_GLRENDER_API CFLRenderGL_GetWorldPos(glm::vec2 screenPos, const glm::mat4& view, const  glm::mat4& perspective, glm::vec4 viewPort);

bool CFL_GLRENDER_API CFLRenderGL_SaveImageByVoid(const void* buffer, unsigned int width, unsigned int height, unsigned int pixsize,
	const std::string& fileName = "e:/saveImageByVoid.png", int stride_bytes = 0, bool isOverTurnY = true);

void CFL_GLRENDER_API CFLRenderGL_DWORD2RGBA(const DWORD& color, CGRGBA& rgba);
void CFL_GLRENDER_API CFLRenderGL_RGBA2DWORD(DWORD& color, const CGRGBA& rgba);

/*
* LineManager
*/
//0.5 < linewidth <10.0
bool CFL_GLRENDER_API CFLRenderGL_SetLineWidth(int Device, float linewidth);



#endif // !_CGRENDERAPI_H_
