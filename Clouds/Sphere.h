#pragma once

#include "GLRenderObject.h"

class Sphere : public GLRenderObject
{
public:
	Sphere() : m_isNowBillboard(false){}

//	Shader* getShader();

private:
	void renderImpl();

	void draw();
	void drawBillboard();

	void createBillboard();

	bool   m_isNowBillboard;

};

