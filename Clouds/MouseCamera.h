#pragma once

#include "Camera.h"

class MouseCamera : public Camera
{
public:
	MouseCamera():m_horAngle(glm::pi<GLfloat>()), m_verAngle(0.0f), m_speed(3.0f){}

	void setAngles(GLfloat horAngle, GLfloat verAngle);

private:
	GLfloat m_horAngle;
	GLfloat m_verAngle;

	GLfloat m_speed;
};

