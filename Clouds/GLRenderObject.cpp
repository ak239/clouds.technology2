#include "stdafx.h"
#include "GLRenderObject.h"


GLRenderObject::GLRenderObject(GLContext _context) : Object(_context), isRendering(true), m_shader(0),
	scale(1.0f), pos(0.0f), m_rotateVec(1.0f, 0.0f, 0.0f), m_angle(0.0f)
{
}

GLRenderObject::~GLRenderObject(void)
{
}

void GLRenderObject::render()
{
	GLContextGetter get(getContext());
	renderImpl();
}

void GLRenderObject::setIsRendering(bool _isRendering)
{
	isRendering = _isRendering;
}

bool GLRenderObject::getIsRendering() const
{
	return isRendering;
}

Shader* GLRenderObject::getShader()
{
	return m_shader;
}

void GLRenderObject::setShader(Shader* shader)
{
	m_shader = shader;
}

void GLRenderObject::move(const glm::vec3& d)
{
	pos += d;
}

void GLRenderObject::setScale(const glm::vec3& _scale)
{
	scale = _scale;
}

void GLRenderObject::setRotate(const glm::vec3& _rotateVec, GLfloat _angle)
{
	m_rotateVec = _rotateVec;
	m_angle     = _angle;
}

glm::mat4 GLRenderObject::model()
{
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !
	//Model = glm::rotate(Model, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	Model = glm::rotate(Model, m_angle * 180.0f / glm::pi<float>(), m_rotateVec);
	Model = glm::scale(Model, scale);
	Model = glm::translate(Model, pos);

	return Model;
}

void GLRenderObject::setColor(const glm::vec3& _color)
{
	m_color = _color;
}

glm::vec3 GLRenderObject::color()
{
	return m_color;
}

glm::vec3 GLRenderObject::position()
{
	return pos;
}




