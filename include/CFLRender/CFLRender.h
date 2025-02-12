#ifndef _CFLRenderGL_H_
#define _CFLRenderGL_H_


#include "CFLRender_Exports.h"
#include "CFLRenderMacros.h"
#include "CFLRenderAPI.h"
#include "CFLglm.h"
#include "CFLPath.h"


#if defined(_MSC_VER) || defined(WIN32) || defined(WIN64)
#ifdef _DEBUG
#pragma comment(lib,"CFLRenderD.lib")
#else
#pragma comment(lib,"CFLRender.lib")
#endif
#endif

#endif // !_CFLRenderGL_H_
