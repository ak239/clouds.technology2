#pragma once

#include "GLRenderObject.h"

class Cube : public GLRenderObject
{
public:
	void init();

private:
	void renderImpl();

	GLuint cubeVAO;
};

