#ifndef _CGPATH_H_
#define _CGPATH_H_

#include "CFLRender_Exports.h"
#include <string>

enum CGPathType
{
	CG_PATH_BIN = 0,
	CG_PATH_PLUGIN,
	CG_PATH_FONTS,
	CG_PATH_CONFIG,
	CG_PATH_COUNT,									//预定义

	CG_PATH_CUSTOM_FIRST = CG_PATH_COUNT,			//自定义
	CG_PATH_MAX_COUNT = CG_PATH_CUSTOM_FIRST + 9,
};

CFL_GLRENDER_API bool CGPath_Init();
CFL_GLRENDER_API void CGPath_Release();

CFL_GLRENDER_API void CGPath_SetPath(CGPathType type, const wchar_t* szPath);
CFL_GLRENDER_API wchar_t* CGPath_GetPath(CGPathType dwType);

CFL_GLRENDER_API std::string wstr2utf8(const std::wstring& wstr);
CFL_GLRENDER_API std::wstring utf82wstr(const std::string& wstr);

CFL_GLRENDER_API bool CGText_Init();//CGPath_Init()包含 更新数据后再次调用
CFL_GLRENDER_API const std::wstring CGText_TextByCode(long code);


CFL_GLRENDER_API void CGLog_INIT(const std::wstring& path);
CFL_GLRENDER_API void CGLog_CLOSE();
CFL_GLRENDER_API void CGTime_START();
CFL_GLRENDER_API void CGTime_END(char*str);


#ifdef DEBUG
#define CFLRenderGL_LOG_INIT(x) do { CGLog_INIT(x); } while(0)
#define CFLRenderGL_LOG_CLOSE do { CGLog_CLOSE(); } while(0)

#define CFLRenderGL_TIME_START do { CGTime_START(); } while(0)
#define CFLRenderGL_TIME_END(x) do { CGTime_END(x); } while(0)
#else
#if 1
#define CFLRenderGL_LOG_INIT(x) do { CGLog_INIT(x); } while(0)
#define CFLRenderGL_LOG_CLOSE do { CGLog_CLOSE(); } while(0)

#define CFLRenderGL_TIME_START do { CGTime_START(); } while(0)
#define CFLRenderGL_TIME_END(x) do { CGTime_END(x); } while(0)
#else
#define CGLOG_INIT(x) 
#define CGLOG_CLOSE 

#define CGTIME_START 
#define CGTIME_END(x) 
#endif // 0
#endif //DEBUG

#endif // !_CGPATH_H_
