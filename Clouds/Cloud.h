#pragma once

#include "RenderObject.h"
#include "Shader.h"
#include "UniformWrapper.h"

class Cloud : public RenderObject
{
public:
	Cloud():m_width(1.0f), m_bar(0){}
	Cloud(GLContext context):RenderObject(context), m_width(1.0f), m_bar(0){}
	~Cloud(){if (m_bar) TwDeleteBar(m_bar); }

	void setMainShader(Shader* mainShader);
	void setWidth(const glm::vec3& width){ m_width = width; generateEllipse(width, 64, 64); }

	void addVarToBar();

private:
	void renderImpl();

	Shader* m_mainShader;
	UniformWrapper<glm::mat4> m_uMVP;
	UniformWrapper<glm::mat4> m_uInvPV;
	UniformWrapper<glm::vec3> m_uWidth;
	UniformWrapper<glm::vec3> m_uCloudPos;

	glm::vec3 m_width;

	TwBar* m_bar;

	void generateEllipse(glm::vec3 width, GLuint rings, GLuint sectors);

	GLuint m_vertexBuffer;
	GLuint m_elementBuffer;
	GLuint m_indicesSize;
};

