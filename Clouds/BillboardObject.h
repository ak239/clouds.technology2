#pragma once

#include "RenderObject.h"
#include "UniformWrapper.h"
#include "Shader.h"

class BillboardObject : public RenderObject
{
public:
	BillboardObject(void);
	BillboardObject(GLContext context);
	~BillboardObject(void){}

	void setBillboardShader(Shader* billboardShader);

private:
	void renderImpl();

	virtual void draw() = 0;
	void drawBillboard();
	void createBillboard();

	Shader* m_billboardShader;

	bool   m_isNowBillboard;
	GLuint m_renderedTexture;

	UniformWrapper<GLuint>    m_texId;
	UniformWrapper<glm::mat4> m_billboardMVP;
	GLuint m_quadVertexbuffer;
};

