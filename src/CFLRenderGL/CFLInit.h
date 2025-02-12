#ifndef _GLINIT_H_
#define _GLINIT_H_

#include "CFLMacros.h"


namespace CFLRenderGL
{
	class GLInit
	{
	public:
		GLInit();
		~GLInit();
		bool initGL();
	private:
		GLInit(const GLInit&) = delete;

	private:
		bool isInit;

	};

	
}
#endif // !_GLINIT_H_
