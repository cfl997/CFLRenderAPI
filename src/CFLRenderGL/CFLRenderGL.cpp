#include "CFLRenderGL.h"



#include "CFLContext.h"
#include "CFLMacros.h"



#include <map>
#include <memory>

#include "CFLInclude.h"

using namespace CFLRenderGL;

template<class Map>
static int GetInsertPos(const Map& InMap)
{
	int id = InMap.size() + 1;
	while (InMap.find(id) != InMap.end())
	{
		id++;
	}

	return id;
}



struct Engine_info
{
	std::map<int, std::unique_ptr<CFLContext>> ContextMap;
	bool isInit = false;
};
Engine_info g_Engine;

static CFLContext* GetContext(int hDevice)
{
	auto it = g_Engine.ContextMap.find(hDevice);
	if (it == g_Engine.ContextMap.end())
	{
		return nullptr;
	}

	return (*it).second.get();
}

bool CFLRenderGL_ENGINE_INIT(int hDevice, void* paramin, void* paramout)
{
	return CFLRenderGL::CGRenderGLInitGL();
}


int CFLRenderGL_createContext(int hDevice, void* paramin, void* paramout)
{
	//if (g_Engine.isInit)
	//	return -1;
	//这个测试用例会有问题？？？

	int newID = GetInsertPos(g_Engine.ContextMap);
	CreateContextIn* createcontextIn = (CreateContextIn*)(paramin);

	g_Engine.ContextMap.emplace(newID, std::make_unique<CFLContext>((HWND)createcontextIn->hwnd, (HGLRC)createcontextIn->glhrc, createcontextIn->width, createcontextIn->height));

	g_Engine.isInit = true;

	*(int*)paramout = newID;
	return newID;
}


bool CFLRenderGL_setViewport(int hDevice, void* paramin, void* paramout)
{
	SetViewport& param = *(SetViewport*)paramin;

	auto context = GetContext(hDevice);
	if (!context)
		return false;

	context->setViewport(param.x, param.y, param.width, param.height);
	return true;
}

bool CFLRenderGL_setScissor(int hDevice, void* paramin, void* paramout)
{
	SetViewport& param = *(SetViewport*)paramin;

	auto context = GetContext(hDevice);
	if (!context)
		return false;

	context->setScissor(param.x, param.y, param.width, param.height);
	return true;
}



bool CFLRenderGL_getViewport(int hDevice, void* paramin, void* paramout)
{
	auto context = GetContext(hDevice);
	if (!context)
		return false;

	CGRECT* rect = (CGRECT*)paramout;
	context->getViewport(rect);

	return true;
}


bool CFLRenderGL_getScissor(int hDevice, void* paramin, void* paramout)
{
	auto context = GetContext(hDevice);
	if (!context)
		return false;

	CGRECT* rect = (CGRECT*)paramout;
	context->getScissor(rect);

	return true;
}


bool CFLRenderGL_renderTest(int hDevice, void* paramin, void* paramout)
{
	auto context = GetContext(hDevice);
	if (!context)
		return false;

	context->renderTest(*(float*)paramin);
	return true;
}



bool CFLRenderGL_Easy_Render(int hDevice, void* paramin, void* paramout)
{
	auto context = GetContext(hDevice);
	if (!context)
		return false;

	GLASSERT(0);
	return true;
}

bool CFLRenderGL_setLineWidth(int hDevice, void* paramin, void* paramout)
{
	auto context = GetContext(hDevice);

	if (!context)
		return false;

	float lineWidth = *(float*)paramin;
	CFLRenderGL::ContextRevert contextRevert(context);
	GLCALL(glLineWidth(lineWidth));
	return true;
}

bool CFLRenderGL_makeCurrent(int hDevice, void* paramin, void* paramout)
{
	auto context = GetContext(hDevice);

	if (!context)
		return false;

	context->makeCurrent();
	return true;
}

bool CFLRenderGL_unMakeCurrent(int hDevice, void* paramin, void* paramout)
{
	auto context = GetContext(hDevice);

	if (!context)
		return false;

	context->unmakeCurrent();
	return true;
}

bool CFLRenderGL_ReleaseContext(int hDevice, void* paramin, void* paramout)
{
	auto context = GetContext(hDevice);
	if (!context)
		return false;
	context->makeCurrent();
	g_Engine.ContextMap.erase(hDevice);
	return true;
}

extern "C"
BOOL pi_main(int ncmd, int hDevice, void* paramin, void* paramout)
{

	switch (ncmd)
	{
	case ENGINE_INIT:
	{
		return CFLRenderGL_ENGINE_INIT(hDevice, paramin, paramout);
		break;
	}
	case ENGINE_CREATE_CONTEXT:
	{
		return CFLRenderGL_createContext(hDevice, paramin, paramout);
		break;
	}
	case ENGINE_SETVIEWPORT:
	{
		return CFLRenderGL_setViewport(hDevice, paramin, paramout);
		break;
	}
	case ENGINE_SETSCISSOR:
	{
		return CFLRenderGL_setScissor(hDevice, paramin, paramout);
		break;
	}
	case ENGINE_GETVIEWPORT:
	{
		return CFLRenderGL_getViewport(hDevice, paramin, paramout);
		break;
	}
	case ENGINE_GETSCISSOR:
	{
		return CFLRenderGL_getScissor(hDevice, paramin, paramout);
		break;
	}
	case ENGINE_RENDER_TEST:
	{
		return CFLRenderGL_renderTest(hDevice, paramin, paramout);
		break;
	}
	case ENGINE_EASY_RENDER:
	{
		return CFLRenderGL_Easy_Render(hDevice, paramin, paramout);
		break;
	}
	case ENGINE_SET_LINEWIDTH:
	{
		return CFLRenderGL_setLineWidth(hDevice, paramin, paramout);
	}
	case ENGINE_MAKECURRENT:
	{
		return CFLRenderGL_makeCurrent(hDevice, paramin, paramout);
	}
	case ENGINE_UNMAKECURRENT:
	{
		return CFLRenderGL_unMakeCurrent(hDevice, paramin, paramout);
	}
	case ENGINE_RELEASE_CONTEXT:
	{
		return CFLRenderGL_ReleaseContext(hDevice, paramin, paramout);
	}
	default:
		break;
	}
	return 0;
}
