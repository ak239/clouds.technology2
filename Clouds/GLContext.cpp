#include "stdafx.h"
#include "GLContext.h"
#include "Utility.h"

GLContextGetter::GLContextGetter(const GLContext& context):oldWindowId(glutGetWindow())
{
	if (context.windowId <= 0) 
		throw new int;
	debugPrint(context.windowId == 0, "Try to use context without context");
	glutSetWindow(context.windowId);
}

GLContextGetter::~GLContextGetter(void)
{
	glutSetWindow(oldWindowId);
}