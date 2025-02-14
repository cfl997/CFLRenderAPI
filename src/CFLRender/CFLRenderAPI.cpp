#include "CFLRenderAPI.h"
#include "CFLRender_Enums.h"
#include "CFLRender_Fonts.h"

#include <Windows.h>
#include <tchar.h>

#include "CFLTime.h"
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../third/stb_image_write.h"

TCHAR g_szPathPlugIn[MAX_PATH] = { 0 };

typedef BOOL(*pi_main)(int ncmd, int hDevice, void* paramin, void* paramout);

struct deviceInfo
{
	pi_main p;
	HANDLE hModule;
};

deviceInfo g_deviceManager;
static bool isInit = true;


bool CFL_GLRENDER_API CFLRenderGL_Init(const TCHAR* pluginPath)
{
	if (!isInit)
		return false;
	isInit = false;
	TCHAR* pDllName = NULL;
#ifdef _DEBUG
	pDllName = L"CFLRenderGLD.DLL";
#else
	pDllName = L"CFLRenderGL.DLL";
#endif

	_tcscpy(g_szPathPlugIn, pluginPath);


	TCHAR szName[512], szPath[512];
	_tcscpy(szPath, g_szPathPlugIn);


	_tprintf(L"%s\n", pDllName);
	_tcscpy(szName, szPath);
	_tcscat(szName, _T("/RenderEngine/"));
	SetDllDirectoryW(szName);//设置查找目录

	//CGLOG_INIT(szName);//外部exe打开

	_tcscat(szName, pDllName);

	CGTIME_START;
	HANDLE hDll = LoadLibrary(szName);
	CGTIME_END("LoadLibrary");

	if (hDll)
	{
		CGTIME_START;
		pi_main p = (pi_main)GetProcAddress((HMODULE)hDll, "pi_main");
		CGTIME_END("GetProcAddress");
		if (!p)
		{
			_tprintf(_T("error:%s\n"), pDllName);
			FreeLibrary((HMODULE)hDll);
			return FALSE;
		}
		if (p(ENGINE_INIT, 0, 0, 0) == FALSE)
		{
			_tprintf(_T("init error:%s\n"), pDllName);
			FreeLibrary((HMODULE)hDll);
			return FALSE;
		}
		_tprintf(_T("init success:%s\n"), pDllName);

		g_deviceManager.hModule = hDll;
		g_deviceManager.p = p;
		return true;
	}
	std::wcout << "error: loadlibrary: " << GetLastError() << std::endl;
	_tprintf(_T("error:%s\n"), pDllName);
	assert(false);

	return false;
}

bool CFL_GLRENDER_API CFLRenderGL_Release()
{
	if (g_deviceManager.hModule)
	{
		g_deviceManager.p(ENGINE_RELEASE, 0, 0, 0);
		FreeLibrary((HMODULE)g_deviceManager.hModule);
		g_deviceManager.hModule = NULL;
	}
	return false;
}

int CFL_GLRENDER_API CFLRenderGL_CreateContext(HWND hWnd, HGLRC hGLRC, int nWidth, int nHeight)
{
	int hNewDeivce = 0;
	CreateContextIn createContextIn{ hWnd,hGLRC ,nWidth,nHeight };
	g_deviceManager.p(ENGINE_CREATE_CONTEXT, 0, &createContextIn, &hNewDeivce);
	return hNewDeivce;
}

bool CFL_GLRENDER_API CFLRenderGL_ReleaseContext(int Device)
{
	return g_deviceManager.p(ENGINE_RELEASE_CONTEXT, Device, nullptr, nullptr);
}

bool CFL_GLRENDER_API CFLRenderGL_MakeCurrent(int Device)
{
	return g_deviceManager.p(ENGINE_MAKECURRENT, Device, nullptr, nullptr);
}

bool CFL_GLRENDER_API CFLRenderGL_UnMakeCurrent(int Device)
{
	return g_deviceManager.p(ENGINE_UNMAKECURRENT, Device, nullptr, nullptr);
}

bool CFL_GLRENDER_API CFLRenderGL_SetViewport(int Device, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	SetViewport setviewport{ x,y,width,height };
	return g_deviceManager.p(ENGINE_SETVIEWPORT, Device, &setviewport, nullptr);
}

bool CFL_GLRENDER_API CFLRenderGL_SetScissor(int Device, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	SetViewport setviewport{ x,y,width,height };
	return g_deviceManager.p(ENGINE_SETSCISSOR, Device, &setviewport, nullptr);
}

bool CFL_GLRENDER_API CFLRenderGL_SetViewport(int Device, CGRECT viewPort)
{
	SetViewport setviewport{ viewPort.x,viewPort.y,viewPort.width,viewPort.height };
	return g_deviceManager.p(ENGINE_SETVIEWPORT, Device, &setviewport, nullptr);
}

bool CFL_GLRENDER_API CFLRenderGL_SetScissor(int Device, CGRECT scissor)
{
	SetViewport setviewport{ scissor.x,scissor.y,scissor.width,scissor.height };
	return g_deviceManager.p(ENGINE_SETSCISSOR, Device, &setviewport, nullptr);
}

bool CFL_GLRENDER_API CFLRenderGL_GetViewport(int Device, CGRECT* rect)
{
	return g_deviceManager.p(ENGINE_GETVIEWPORT, Device, nullptr, rect);
}

bool CFL_GLRENDER_API CFLRenderGL_GetScissor(int Device, CGRECT* rect)
{
	return g_deviceManager.p(ENGINE_GETSCISSOR, Device, nullptr, rect);
}

bool CFL_GLRENDER_API CFLRenderGL_RenderTest(int Device, float time)
{
	return g_deviceManager.p(ENGINE_RENDER_TEST, Device, &time, nullptr);
}

bool CFL_GLRENDER_API CFLRenderGL_Render(int hDevice, int hVertexBuffer, int hIndexBuffer, int hVertexShader, int hGeometryShader, int hPixShader, int nStart, int nVertexCount, int nVertexBase)
{
	EasyRenderIn param;
	param.hVertexBuffer = hVertexBuffer;
	param.hIndexBuffer = hIndexBuffer;

	param.hVertexShader = hVertexShader;
	param.hGeometryShader = hGeometryShader;
	param.hPixShader = hPixShader;

	param.nStart = nStart;
	param.nVertexCount = nVertexCount;
	param.nVertexBase = nVertexBase;

	return g_deviceManager.p(ENGINE_EASY_RENDER, hDevice, &param, NULL);
}

static glm::vec3 screenToWorld(glm::vec3 win, glm::mat4 view, glm::mat4 project, glm::vec4 viewport)
{
	glm::mat4 Inverse = glm::inverse(project * view);

	glm::vec4 tmp = glm::vec4(win, float(1));
	tmp.x = (tmp.x - float(viewport[0])) / float(viewport[2]);
	tmp.y = (tmp.y - float(viewport[1])) / float(viewport[3]);
	tmp = tmp * static_cast<float>(2) - static_cast<float>(1);

	glm::vec4 obj = Inverse * tmp;
	obj /= obj.w;

	return glm::vec3(obj);
}
static glm::vec3 screenToWorld1(glm::vec3 win, glm::mat4 view, glm::mat4 projection, glm::vec4 viewport) {
	glm::vec4 viewportCoord = glm::vec4(win.x, viewport.w - win.y, win.z, 1.0);
	glm::vec4 clipCoord;
	clipCoord.x = (2.0f * (viewportCoord.x - viewport.x)) / viewport.z - 1.0f;
	clipCoord.y = (2.0f * (viewportCoord.y - viewport.y)) / viewport.w - 1.0f;
	clipCoord.z = 2.0f * viewportCoord.z - 1.0f;
	clipCoord.w = 1.0f;

	glm::mat4 inverseProjection = glm::inverse(projection);
	glm::vec4 eyeCoord = inverseProjection * clipCoord;
	eyeCoord.z = -1.0f;

	glm::mat4 inverseView = glm::inverse(view);
	glm::vec4 worldCoord = inverseView * eyeCoord;

	worldCoord /= worldCoord.w;

	return glm::vec3(worldCoord.x, -worldCoord.y, worldCoord.z);
}
glm::vec3 CFL_GLRENDER_API CFLRenderGL_GetWorldPos(glm::vec2 screenPos, const glm::mat4& viewMatrix, const glm::mat4& perspectiveMatrix, glm::vec4 viewPort)
{
	static const float z = 1.0f;//must
	return screenToWorld(glm::vec3(screenPos.x, viewPort.w - screenPos.y, z), viewMatrix, perspectiveMatrix, viewPort);
}

/*
* 描述:保存图像，支持的图像文件格式包括PNG、BMP、TGA、JPG、HDR
*
* 参数：
*   buffer：指定期望的通道数，若为0则不做颜色空间变换
*   width:图像宽
*   height:图像高
*   pixsize:图像通道数
*   fileName:保存图像名
*   stride_bytes:步长，若为0则为宽*通道数，仅限png
*	isOverTurnY:是否翻转Y
*
*返回值：成功返回非0值，否则返回0。
*/
bool CFLRenderGL_SaveImageByVoid(const void* buffer, unsigned int width, unsigned int height, unsigned int pixsize, const std::string& fileName, int stride_bytes, bool isOverTurnY)
{
	if (isOverTurnY)
		stbi_flip_vertically_on_write(1);
	else
		stbi_flip_vertically_on_write(0);
	return stbi_write_png(fileName.c_str(), width, height, pixsize, buffer, stride_bytes);
}

void CFLRenderGL_DWORD2RGBA(const DWORD& color, CGRGBA& rgba)
{
	const float f = 1.f / 255.f;
	rgba.R = f * (float((color >> 16) & 0xFF));
	rgba.G = f * (float((color >> 8) & 0xFF));
	rgba.B = f * (float((color >> 0) & 0xFF));
	rgba.A = f * (float((color >> 24) & 0xFF));
}

void CFLRenderGL_RGBA2DWORD(DWORD& color, const CGRGBA& rgba)
{
	DWORD dwR = rgba.R >= 1.0f ? 0xff : rgba.R <= 0.0f ? 0x00 : (DWORD)(rgba.R * 255.0f + 0.5f);
	DWORD dwG = rgba.G >= 1.0f ? 0xff : rgba.G <= 0.0f ? 0x00 : (DWORD)(rgba.G * 255.0f + 0.5f);
	DWORD dwB = rgba.B >= 1.0f ? 0xff : rgba.B <= 0.0f ? 0x00 : (DWORD)(rgba.B * 255.0f + 0.5f);
	DWORD dwA = rgba.A >= 1.0f ? 0xff : rgba.A <= 0.0f ? 0x00 : (DWORD)(rgba.A * 255.0f + 0.5f);
	//color = (dwA << 24) | (dwR << 16) | (dwG << 8) | dwB;
	color = (dwA << 24) | (dwB << 16) | (dwG << 8) | dwR;
}

bool CFL_GLRENDER_API CFLRenderGL_SetLineWidth(int Device, float linewidth)
{
	return g_deviceManager.p(ENGINE_SET_LINEWIDTH, Device, &linewidth, NULL);
}

glm::vec3 CFLRenderGL_GetPosRay(glm::vec2 screenPos, const glm::mat4& view, const glm::mat4& perspective, glm::vec4 viewPort)
{
	glm::vec3 farPos = screenToWorld(glm::vec3(screenPos.x, viewPort.w - screenPos.y, 0.0f), view, perspective, viewPort);
	glm::vec3 nearPos = screenToWorld(glm::vec3(screenPos.x, viewPort.w - screenPos.y, 1.0f), view, perspective, viewPort);
	return glm::normalize(farPos - nearPos);
}


