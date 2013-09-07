#pragma once

#include "GLRenderObject.h"

class Plane : public GLRenderObject
{
public:
	void init();

private:
	void renderImpl();

	GLuint vertexBuffer;
};

