#ifndef _CFLINCLUDE_H_
#define _CFLINCLUDE_H_


#if defined(__unix__)
#	define _OPENGL_UNIX_
#elif defined(WIN32) || defined(WIN64)
#	define _OPENGL_WIN_
#endif

#include "../third/glew-2.2.0/include/GL/glew.h"
#include "GL/gl.h"

#if defined(_OPENGL_WIN_)
#include "../third/glew-2.2.0/include/GL/wglew.h"
#endif

#include <iostream>
#include <assert.h> 
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>

#if _DEBUG
#	if defined(_OPENGL_UNIX_)
#		define MP_BREAK() __builtin_trap();
#	elif defined(_OPENGL_WIN_)
#		define MP_BREAK() __debugbreak();
#	endif
#else
#	define MP_BREAK() ;
#endif

#define INT2VOIDP(i) (void*)(uintptr_t)(i)
#define BUFFER_OFFSET(i) INT2VOIDP(i)

#if _DEBUG
#	ifndef GLASSERT
#		define GLASSERT(X) if(!(X))MP_BREAK()
#	endif
#else
#	ifndef GLASSERT
#		define GLASSERT(X) ((void)0)
#	endif
#endif // _DEBUG




#ifdef _DEBUG
#define GLCALL(x) GLClearError();\
	x;\
	GLASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCALL(x) x
#endif
void printError(GLenum error);

void GLClearError();
bool GLCheckError();

static void printErrorFileLine(const char* function, const char* file, int line)
{
	std::cout << "(" << "error" << ") : " << function << "file : " << file <<
		"line:" << line << std::endl;
	MP_BREAK();
}

/*
 *	���еķ���ֵ�У�
 *	GL_NO_ERROR  ����0����ǰ�޴���ֵ
 *	GL_INVALID_ENUM  ����1280������ʹ�÷Ƿ�ö�ٲ���ʱ�����ʹ�øò�����ָ���������򷵻� GL_INVALID_OPERATION
 *	GL_INVALID_VALUE  ����1281������ʹ�÷Ƿ�ֵ����ʱ�����ʹ�øò�����ָ���������򷵻� GL_INVALID_OPERATION
 *	GL_INVALID_OPERATION  ����1282�������״̬���϶���ָ���Ĳ����Ƿ���
 *	GL_STACK_OVERFLOW  ����1283��ѹջ����������ջ��С��
 *	GL_STACK_UNDERFLOW  ����1284����ջ�����ﵽ��ջ�ײ���
 *	GL_OUT_OF_MEMORY  ����1285�����ܷ����㹻�ڴ�ʱ��
 *	GL_INVALID_FRAMEBUFFER_OPERATION  ����1286��������δ׼���õ��滺��ʱ��
 *	GL_CONTEXT_LOST  ����1287�������Կ����õ��� OpenGL context ��ʧ��
 *	cfl-2025/02/12
 */

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		printError(error);
		std::cout << "(" << error << ") : " << function << "file : " << file <<
			"line:" << line << std::endl;
		return false;
	}
	return true;
}


void setGLEnable(bool isEnable, GLenum mode);

typedef HWND HWindow;
typedef HDC HDisplay;
typedef int HVisualConfig;
typedef HGLRC HGLContext;

enum PrimitiveMode
{
	LINES = GL_LINES,
	LINE_STRIP = GL_LINE_STRIP,
	TRIANGLES = GL_TRIANGLES,
	TRIANGLE_STRIP = GL_TRIANGLE_STRIP,

	PRIMITIVEMODE_UNKOWN = 0xFFFFFFFF,
};


#endif  //_CFLINCLUDE_H_