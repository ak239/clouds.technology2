#pragma once

#include "GLRenderObject.h"

class Axis : public GLRenderObject
{
public:
	void init();

private:
	void renderImpl();

	GLuint vertexBuffer;
};

