#include "CFLInclude.h"

void printError(GLenum error)
{
	std::cout << "[OpenGL Error] ";
	switch (error) {
	case GL_INVALID_ENUM:
		std::cout << "GL_INVALID_ENUM : 仅当使用非法枚举参数时，如果使用该参数有指定环境，则返回 GL_INVALID_OPERATION.";
		break;
	case GL_INVALID_VALUE:
		std::cout << "GL_INVALID_VALUE : 数值参数超出范围.";
		break;
	case GL_INVALID_OPERATION:
		std::cout << "GL_INVALID_OPERATION : 当前状态不允许指定的操作.";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION : framebuffer对象未完成.";
		break;
	case GL_OUT_OF_MEMORY:
		std::cout << "GL_OUT_OF_MEMORY : 内存不足，无法执行该命令.";
		break;
	case GL_STACK_UNDERFLOW:
		std::cout << "GL_STACK_UNDERFLOW : 尝试执行可能导致内部堆栈下溢的操作.";
		break;
	case GL_STACK_OVERFLOW:
		std::cout << "GL_STACK_OVERFLOW : 尝试执行可能导致内部堆栈溢出的操作.";
		break;
	default:
		std::cout << "Unrecognized error" << error;
	}
	std::cout << std::endl;
}

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError()
{
	while (GLenum error = glGetError())
	{

		printError(error);
		return false;
	}
	return true;
}



void setGLEnable(bool isEnable, GLenum mode)
{
	if (isEnable)
	{
		if (!glIsEnabled(mode))
		{
			GLCALL(glEnable(mode));
		}
	}
	else
	{
		if (glIsEnabled(mode))
		{
			GLCALL(glDisable(mode));
		}
	}
}
