#pragma once

#include "RenderObject.h"
#include "Shader.h"

class Particle : public RenderObject
{
public:
	Particle():m_shader(0),m_vertexBuffer(0),m_texId(0){}
	Particle(GLContext context, Shader* shader, GLuint vertexBuffer, GLuint texId);
	~Particle(){};

	void init();
	void setOffset(const glm::vec3& _offset){ m_offset = _offset; }
	void setCenter(const glm::vec3& _center){ m_center = _center; }
	void setWidth(GLfloat _width){ m_width = _width; }

	const glm::vec3& getOffset() const{ return m_offset; }

	static Shader* LoadShader(const GLContext& context);
	static GLuint  GetVertexBuffer(const GLContext& context);
	static GLuint  GetTextureID(const GLContext& context);

private:

	void renderImpl();

	Shader* m_shader;

	GLuint m_vertexBuffer;
	GLuint m_texId;

	glm::vec3 m_offset;
	glm::vec3 m_center;
	GLfloat   m_width;

	UniformWrapper<glm::mat4> m_MVP;
	UniformWrapper<glm::vec4> m_rightNormal;
	UniformWrapper<glm::vec4> m_upNormal;
};

