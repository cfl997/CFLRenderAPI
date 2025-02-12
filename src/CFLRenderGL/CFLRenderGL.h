#ifndef _CFLRENDERGL_H_
#define _CFLRENDERGL_H_


#include <windows.h>
#include <tchar.h>

#include <string>

typedef std::basic_string<TCHAR> tstring;

extern "C" BOOL pi_main(int ncmd, int hDevice, void* paramin, void* paramout);


#endif // !_CFLRENDERGL_H_
