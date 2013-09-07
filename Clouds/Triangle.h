#pragma once

#include "GLRenderObject.h"

class Triangle : public GLRenderObject
{
public:
	void init();

private:
	void renderImpl();

	GLuint vertexBuffer;
};

