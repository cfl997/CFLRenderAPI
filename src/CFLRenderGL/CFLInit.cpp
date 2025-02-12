#include "CFLInit.h"




#include "CFLInclude.h"

#include "CFLTime.h"

using namespace CFLRenderGL;



static LRESULT CALLBACK PlatformDummyGLWndproc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, Message, wParam, lParam);
}

PFNWGLCREATECONTEXTATTRIBSARBPROC _wglCreateContextAttribsARB = NULL;
PFNWGLSWAPINTERVALEXTPROC _wglSwapIntervalEXT = NULL;

HWindow CreateDummyWindow()
{

	const TCHAR* WindowClassName = TEXT("DummyGLWindow");

	static bool bInitializedWindowClass = false;
	if (!bInitializedWindowClass)
	{
		WNDCLASS wc;

		bInitializedWindowClass = true;
		memset(&wc, 0, sizeof(wc));
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = PlatformDummyGLWndproc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = NULL;
		wc.hIcon = NULL;
		wc.hCursor = NULL;
		wc.hbrBackground = (HBRUSH)(COLOR_MENUTEXT);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = WindowClassName;
		ATOM ClassAtom = ::RegisterClass(&wc);
	}

	HWindow WindowHandle = CreateWindowEx(
		WS_EX_WINDOWEDGE,
		WindowClassName,
		NULL,
		WS_POPUP,
		0, 0, 1, 1,
		NULL, NULL, NULL, NULL);


	return WindowHandle;
}

int SetupDefaultDC(HDisplay dc)
{

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size
		1,                          // version
		PFD_SUPPORT_OPENGL |        // OpenGL window
		PFD_DRAW_TO_WINDOW |        // render to window
		PFD_DOUBLEBUFFER,           // support double-buffering
		PFD_TYPE_RGBA,              // color type
		32,                         // prefered color depth
		0, 0, 0, 0, 0, 0,           // color bits (ignored)
		0,                          // no alpha buffer
		0,                          // alpha bits (ignored)
		0,                          // no accumulation buffer
		0, 0, 0, 0,                 // accum bits (ignored)
		0,                          // depth buffer  //cfl-20240619
		0,                          // no stencil buffer
		0,                          // no auxiliary buffers
		PFD_MAIN_PLANE,             // main layer
		0,                          // reserved
		0, 0, 0,                    // no layer, visible, damage masks
	};

	int format = ChoosePixelFormat(dc, &pfd);
	auto result = SetPixelFormat(dc, format, &pfd);
	/*
		if (result != 1)
		{
			DWORD err = GetLastError();

			return 0;
		}
	*/
	return format;
}

bool initGLA()
{


	{
		CGTIME_START;
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			MessageBox(NULL, L"无法初始化GLEW！", L"错误", MB_ICONERROR | MB_OK);
			return false;
		}
		CGTIME_END("glewInit");
		return true;
	}
	CGTIME_START;
	const TCHAR* WindowClassName = TEXT("DummyGLWindow");

	static bool bInitializedWindowClass = false;
	if (!bInitializedWindowClass)
	{
		WNDCLASS wc;

		bInitializedWindowClass = true;
		memset(&wc, 0, sizeof(wc));
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = PlatformDummyGLWndproc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = NULL;
		wc.hIcon = NULL;
		wc.hCursor = NULL;
		wc.hbrBackground = (HBRUSH)(COLOR_MENUTEXT);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = WindowClassName;
		ATOM ClassAtom = ::RegisterClass(&wc);
	}

	HWindow WindowHandle = CreateWindowEx(
		WS_EX_WINDOWEDGE,
		WindowClassName,
		NULL,
		WS_POPUP,
		0, 0, 1, 1,
		NULL, NULL, NULL, NULL);

	CGTIME_END("CreateWindowEx");

	CGTIME_START;
	// 获取设备上下文
	HDC hdc = GetDC(WindowHandle);
	CGTIME_END("GetDC-WindowHandle");

	CGTIME_START;
	HWindow dummyWindow = CreateDummyWindow();
	CGTIME_END("CreateDummyWindow");


	CGTIME_START;
	HDC dc = ::GetDC(dummyWindow);
	CGTIME_END("GetDC-dummyWindow");
	CGTIME_START;
	SetupDefaultDC(dc);
	CGTIME_END("SetupDefaultDC");
	CGTIME_START;
	HGLRC dummyContext = wglCreateContext(dc);
	CGTIME_END("wglCreateContext");


	if (wglMakeCurrent(dc, dummyContext))
	{
		_wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		_wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		CGTIME_START;
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			MessageBox(NULL, L"无法初始化GLEW！", L"错误", MB_ICONERROR | MB_OK);
			return false;
		}
		CGTIME_END("glewInit");

	}

	CGTIME_START;
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(dummyContext);

	ReleaseDC(dummyWindow, dc);
	DestroyWindow(dummyWindow);
	CGTIME_END("releaseDC-Window");
	return true;
}

GLInit::GLInit() :isInit(false)
{

}

GLInit::~GLInit()
{

}


bool CFLRenderGL::GLInit::initGL()
{
	isInit = initGLA();
	assert(isInit);
	return isInit;
}


