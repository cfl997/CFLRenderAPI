#include "CFLContext.h"

#include "CFLInclude.h"

#include "CFLInit.h"




using namespace CFLRenderGL;






static PrimitiveMode ToGLPrimitiveMode(GLPrimitiveTypes Mode)
{
	PrimitiveMode primitiveMode = PRIMITIVEMODE_UNKOWN;
	switch (Mode)
	{
	case LINE_LIST:
		primitiveMode = PrimitiveMode::LINES;
		break;
	case LINE_LIST_STRIP:
		primitiveMode = PrimitiveMode::LINE_STRIP;
		break;
	case TRIANGLELIST:
		primitiveMode = PrimitiveMode::TRIANGLES;
		break;
	case TRIANGLESTRIP:
		primitiveMode = PrimitiveMode::TRIANGLE_STRIP;
		break;
	case PRIMITIVE_UNKOWN:
		break;
	default:
		break;
	}
	return primitiveMode;
}

struct CFLContext::PrivateData
{
	CGRECT viewport;
	CGRECT scissor;

	GLuint m_hFrameBuffer = 0;
	GLuint m_hInterReadFB = 0;
	/*
	* test
	*/

	bool isFirst = true;
	unsigned int VBO, vao, EBO;
	unsigned int shaderpipeline;

};

/*
 * 设置GL像素格式
 */
static bool SetupPixelFormat(HDC dc)
{
	//像素格式描述符,渲染的像素格式
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	int pixelformat = 0;
	if ((pixelformat = ChoosePixelFormat(dc, &pfd)) == 0)
		return false;

	if (SetPixelFormat(dc, pixelformat, &pfd) == FALSE)
		return false;

	return true;
}





CFLRenderGL::CFLContext::CFLContext(HWND hWnd, HGLRC hGLRC, int nWidth, int nHeight) :m_priv(new PrivateData)
{



	setWindowData(hWnd, hGLRC, nWidth, nHeight);
	makeCurrent();


	auto& d = *m_priv;

	GLCALL(glGenFramebuffers(1, &d.m_hFrameBuffer));
	GLCALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d.m_hFrameBuffer));
	GLCALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));//默认读为0

	unmakeCurrent();
}
CFLContext::~CFLContext()
{

	auto& d = *m_priv;



	wglDeleteContext(m_hGLRC);
	ReleaseDC(m_hWnd, m_hDC);


	if (m_priv)
	{
		delete m_priv;
		m_priv = nullptr;
	}
}



void CFLRenderGL::CFLContext::renderTest(float pos)
{

	ContextRevert contextRevert(this);
	auto& d = *m_priv;
	//makeCurrent();
	//
	//在此使用OpenGL进行绘制
	//
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);


	{
		// 开启深度测试
		GLCALL(glEnable(GL_DEPTH_TEST));

		GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		// 在这里绘制您的 OpenGL 场景
		float index = 1.0f;
		if (pos != 0)
		{
			index = pos;
		}
		if (pos != 0.0f)
		{
			GLCALL(glClearColor(1.0f, 0.0f, 0.0f, 1.0f));
			GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
			GLCALL(glBegin(GL_TRIANGLES));
			GLCALL(glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f));
			GLCALL(glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(index, -1.0f, 0.0f));
			GLCALL(glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f));
			glEnd();
			GLCALL(glFlush());
		}

	}

	//swapBuffer();

	//unmakeCurrent();

}


void CFLRenderGL::CFLContext::renderTest(int textureid, int vsshader, int psshader, int vertexBuffer, int indexBuffer)
{
	ContextRevert contextRevert(this);


	auto& d = *m_priv;
	//auto texture = d.textures[textureid];





	if (d.isFirst)
	{
		d.isFirst = false;
		//float vertices[] = {
		//	// positions          // colors           // texture coords
		//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		//};

		float vertices[] = {
			// positions          // colors           // texture coords
			 1.f,  1.f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			 1.f, -1.f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-1.f, -1.f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-1.f,  1.f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};
		glGenVertexArrays(1, &d.vao);
		//glGenBuffers(1, &d.VBO);
		//glGenBuffers(1, &d.EBO);

		glBindVertexArray(d.vao);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
		//glEnableVertexAttribArray(2);

		/*
		* shaderPipeline
		*/
		glCreateProgramPipelines(1, &d.shaderpipeline);
		GLCALL(glBindProgramPipeline(d.shaderpipeline));



	}

	// render
	// ------
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind textures on corresponding texture units
	GLCALL(glActiveTexture(GL_TEXTURE0));
	GLCALL(glBindTexture(GL_TEXTURE_2D, textureid));


	GLCALL(glUseProgramStages(d.shaderpipeline, 1, vsshader));
	GLCALL(glUseProgramStages(d.shaderpipeline, 2, psshader));
	// render container
	glBindVertexArray(d.vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	swapBuffer();


}




bool CFLRenderGL::CFLContext::setHWND(HWND hWnd, HGLRC hShareContext, int nWidth, int nHeight)
{
	m_hWnd = hWnd;
	m_hDC = ::GetDC(m_hWnd);
	win_width = nWidth;
	win_height = nHeight;


	bool setpixelFormatTrue = SetupPixelFormat(m_hDC);
	assert(setpixelFormatTrue);

	//assert(_wglCreateContextAttribsARB);
	assert(wglCreateContextAttribsARB);

	int DebugFlag = 0;
	int Profile = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
	Profile = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
#ifdef _DEBUG
	DebugFlag = WGL_CONTEXT_DEBUG_BIT_ARB;
#endif
	int AttribList[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | DebugFlag,
		WGL_CONTEXT_PROFILE_MASK_ARB,  Profile,
		0
	};

	m_hGLRC = wglCreateContextAttribsARB(m_hDC, hShareContext, AttribList);

	if (!m_hGLRC)
	{
		auto err = GetLastError();
		assert(false);

		return false;
	}

	m_bInit = true;
	return true;
}

void CFLRenderGL::CFLContext::setWindowData(HWND hWnd, HGLRC hGLRC, int nWidth, int nHeight)
{
	m_hWnd = hWnd;
	m_hDC = ::GetDC(m_hWnd);
	m_hGLRC = hGLRC;
	win_width = nWidth;
	win_height = nHeight;
}

HGLRC CFLRenderGL::CFLContext::getShareGLContext()
{
	return m_hGLRC;
}

void CFLRenderGL::CFLContext::Resize(int nWidth, int nHeight)
{

}



void CFLRenderGL::CFLContext::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	ContextRevert contextRevert(this);
	auto& d = *m_priv;
	d.viewport.x = x;
	d.viewport.y = y;
	d.viewport.width = width;
	d.viewport.height = height;

	GLCALL(glViewport(x, y, width, height));
}

void CFLRenderGL::CFLContext::setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	ContextRevert contextRevert(this);
	auto& d = *m_priv;
	d.scissor.x = x;
	d.scissor.y = y;
	d.scissor.width = width;
	d.scissor.height = height;

	GLCALL(glScissor(x, y, width, height));
}

void CFLRenderGL::CFLContext::getViewport(CGRECT* rect)
{
	auto& d = *m_priv;
	rect->x = d.viewport.x;
	rect->y = d.viewport.y;
	rect->width = d.viewport.width;
	rect->height = d.viewport.height;
}

void CFLRenderGL::CFLContext::getScissor(CGRECT* rect)
{
	auto& d = *m_priv;
	rect->x = d.scissor.x;
	rect->y = d.scissor.y;
	rect->width = d.scissor.width;
	rect->height = d.scissor.height;
}

void CFLRenderGL::CFLContext::makeCurrent()
{
	if (m_isCurrentBind)
		return;

	HDC preHDC = wglGetCurrentDC();
	HGLContext preHGLRC = wglGetCurrentContext();

	HDC dc = GetDC(m_hWnd);

	//cfl-todo 更改
	//if (dc != m_hDC /*|| preHGLRC != m_hGLRC*/)
	//{
	//	GLASSERT(0);
	//}

	bool result = wglMakeCurrent(m_hDC, m_hGLRC);
	if (!result)
	{

		//GLCheckError();
		//GLASSERT(0);
	}
	m_isCurrentBind = true;
}

void CFLRenderGL::CFLContext::unmakeCurrent()
{
	wglMakeCurrent(NULL, NULL);
	m_isCurrentBind = false;
}

void CFLRenderGL::CFLContext::swapBuffer()
{
	SwapBuffers(m_hDC);
	//wglSwapLayerBuffers(m_hDC, WGL_SWAP_MAIN_PLANE);
}

bool CFLRenderGL::CGRenderGLInitGL()
{
	GLInit glinit;
	return glinit.initGL();
}
