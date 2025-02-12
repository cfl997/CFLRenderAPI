#ifndef _GLCONTEXT_H_
#define _GLCONTEXT_H_


#include "CFLMacros.h"

#include <Windows.h>

#include <string>


class ContextRevert;

namespace CFLRenderGL
{
	struct RGBA;
	class CFLContext
	{
	public:
		CFLContext(HWND hWnd, HGLRC m_hGLRC, int nWidth = 0, int nHeight = 0);
		~CFLContext();


	private:
		bool setHWND(HWND hWnd, HGLRC hShareContext = 0, int nWidth = 0, int nHeight = 0);
	public:
		/*
		* 窗口
		*/
		//将glfw传入
		void setWindowData(HWND hWnd, HGLRC m_hGLRC, int nWidth = 0, int nHeight = 0);

		HGLRC getShareGLContext();

		void Resize(int nWidth, int nHeight);

	public:
		void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void getViewport(CGRECT* rect);
		void getScissor(CGRECT* rect);

	public:
		void makeCurrent();
		void unmakeCurrent();
		void swapBuffer();
	private:
		bool m_isCurrentBind = false;
	public:
		/*
		* test
		*/
		void renderTest(float pos);//3.3版本
		void renderTest(int textureid, int vsshader, int psShader, int vertexBuffer, int indexBuffer);

	private:
		CFLContext(HWND hWnd, int nWidth, int nHeight);
		CFLContext(const CFLContext&) = delete;
	private:
		HGLRC		m_hGLRC;    //OpenGL渲染上下文环境句柄
		HDC			m_hDC;		//Windows设备环境句柄
		HWND		m_hWnd;
		int win_width;
		int win_height;

		bool	m_bInit;
	private:
		CFLContext() = delete;
		struct PrivateData;
		PrivateData* m_priv;
	};

	class ContextRevert
	{
	public:
		ContextRevert(CFLContext* GLcontext)
		{
			if (GLcontext)
			{
				glcontext = GLcontext;
				GLcontext->makeCurrent();
			}
		}
		~ContextRevert()
		{
			glcontext->unmakeCurrent();
		}
	private:
		ContextRevert() = delete;
		ContextRevert(const ContextRevert&) = delete;

		CFLContext* glcontext = nullptr;
	};

	bool CGRenderGLInitGL();
}

#endif // !_GLCONTEXT_H_
