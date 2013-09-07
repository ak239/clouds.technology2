#include "stdafx.h"
#include "MouseCamera.h"

void MouseCamera::setAngles(GLfloat horAngle, GLfloat verAngle)
{
	glm::vec3 dir(
		cos(verAngle) * sin(horAngle),
		sin(verAngle),
		cos(verAngle) * cos(horAngle)
	);

	glm::vec3 right = glm::vec3(
		sin(horAngle - glm::pi<GLfloat>()/2.0f),
		0,
		cos(horAngle - glm::pi<GLfloat>()/2.0f)
	);

	glm::vec3 up = glm::cross( right, dir );

	setLookAt(dir + getPos());
	setHead(up);
}

