#pragma once

#include <vector>
//#include <pair>

struct ParticleEllipse
{
	GLfloat m_cos;
	GLfloat m_firstAngle;
	GLfloat m_stepAngle;

	ParticleEllipse():m_cos(0.0f), m_firstAngle(0.0f), m_stepAngle(0.0f){}
	ParticleEllipse(GLfloat _cos, GLfloat _firstAngle, GLfloat _stepAngle):
		m_cos(_cos), m_firstAngle(_firstAngle), m_stepAngle(_stepAngle){}
};

class ParticleField
{
public:
	ParticleField(GLfloat a, GLfloat b, GLfloat c):m_a(a), m_b(b), m_c(c){}
	~ParticleField(void){}

	std::pair<std::vector<ParticleEllipse>, GLfloat> generate(GLuint N, GLint maxTry = 10);

private:
	GLfloat m_a, m_b, m_c;
};

