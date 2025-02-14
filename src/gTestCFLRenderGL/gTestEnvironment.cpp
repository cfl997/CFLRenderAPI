#include "gTestInclude.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


#include "tchar.h"


class gTestEnviroment : public testing::Environment
{
public:
	gTestEnviroment()
	{
		TCHAR szFilePath[MAX_PATH + 1] = { 0 };
		GetModuleFileName(NULL, szFilePath, MAX_PATH);
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // ɾ���ļ�����ֻ���·���ִ�
		_tcscat(szFilePath, L"plugin");

		bool isInit = CGRender_Init(szFilePath);//�Ƿ����glew32.dll
		EXPECT_EQ(isInit, true);

		glfwWindow = glfwCreateWindow(0, WINDOW_WIDTH, WINDOW_HEIGHT, "gtest-glfw", 0, 0);


		glfwMakeContextCurrent(glfwWindow);

		{
			// ��ȡ���ھ��
			hwnd = glfwGetWin32Window(glfwWindow);
			HGLRC hGLRC = glfwGetWGLContext(glfwWindow);
			glContextID = CGRender_CreateContext(hwnd, hGLRC, WINDOW_WIDTH, WINDOW_HEIGHT);
		}
	}
	~gTestEnviroment()
	{

	}
	// Override this to define how to set up the environment.
	virtual void SetUp()
	{
		std::cout << "gtestenviroment setup" << std::endl;
	}

	// Override this to define how to tear down the environment.
	virtual void TearDown()
	{
		std::cout << "gtestenviroment tearDown" << std::endl;
	}
public:
	GLFWwindow* glfwWindow = nullptr;
	HWND hwnd;
	int glContextID;
private:

};
testing::Environment* const g_env = testing::AddGlobalTestEnvironment(new gTestEnviroment);