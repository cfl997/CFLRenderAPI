#pragma once


#ifdef CFL_GLRENDER_EXPORTS
#define CFL_GLRENDER_API __declspec(dllexport)
#else
#define  CFL_GLRENDER_API __declspec(dllimport)
#endif
