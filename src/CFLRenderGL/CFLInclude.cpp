#include "CFLInclude.h"

void printError(GLenum error)
{
	std::cout << "[OpenGL Error] ";
	switch (error) {
	case GL_INVALID_ENUM:
		std::cout << "GL_INVALID_ENUM : ����ʹ�÷Ƿ�ö�ٲ���ʱ�����ʹ�øò�����ָ���������򷵻� GL_INVALID_OPERATION.";
		break;
	case GL_INVALID_VALUE:
		std::cout << "GL_INVALID_VALUE : ��ֵ����������Χ.";
		break;
	case GL_INVALID_OPERATION:
		std::cout << "GL_INVALID_OPERATION : ��ǰ״̬������ָ���Ĳ���.";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION : framebuffer����δ���.";
		break;
	case GL_OUT_OF_MEMORY:
		std::cout << "GL_OUT_OF_MEMORY : �ڴ治�㣬�޷�ִ�и�����.";
		break;
	case GL_STACK_UNDERFLOW:
		std::cout << "GL_STACK_UNDERFLOW : ����ִ�п��ܵ����ڲ���ջ����Ĳ���.";
		break;
	case GL_STACK_OVERFLOW:
		std::cout << "GL_STACK_OVERFLOW : ����ִ�п��ܵ����ڲ���ջ����Ĳ���.";
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
