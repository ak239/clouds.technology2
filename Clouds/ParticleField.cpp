#include "stdafx.h"
#include "ParticleField.h"

#include <iostream>

std::pair<std::vector<ParticleEllipse>, GLfloat> ParticleField::generate(GLuint N, GLint maxTry)
{
	GLfloat stepCur   = 0.0f * m_c;
	GLfloat stepReal  = 0.2f * m_c;
	GLfloat pCur      = 1.0f;
	GLfloat stepCos   = 0.1f;
	const GLfloat kt1 = 4.0f * (glm::pi<GLfloat>() * m_a * m_b + (m_a - m_b) * (m_a - m_b)) / (m_a + m_b);

	do{
		stepCos = (stepReal + stepCur) / 2.0f / m_c;
		stepCur = stepCos * m_c;

		pCur = 0.0f;
		for (GLfloat valCos = -1.0f + stepCos; valCos <= 1.0f - stepCos; valCos += stepCos)
			pCur += glm::sqrt<GLfloat>(1.0f - valCos*valCos);
		pCur *= kt1;

		stepReal = pCur / N;
		maxTry--;
	} while (glm::abs(stepReal - stepCur) > 0.01f && maxTry > 0);

	const GLfloat kt2 = kt1 / pCur * N;
	std::vector<ParticleEllipse> output;
	output.push_back(ParticleEllipse(-1.0f, 0.0f, 2 * glm::pi<GLfloat>()));

	bool flg = true;
	for (GLfloat valCos = -1.0f + stepCos; valCos <= 1.0f; valCos += stepCos){
		GLfloat val   = glm::sqrt<GLfloat>(1.0f - valCos*valCos) * kt2;
		GLint   nCur  = glm::ceil(val);

		GLfloat step  = 2 * glm::pi<GLfloat>() / nCur;
		GLfloat first = flg ? 0.0f : step / 2.0f;
		flg = !flg;

		output.push_back(ParticleEllipse(valCos, first, step));
	}
	output.push_back(ParticleEllipse(1.0f, 0.0f, 2 * glm::pi<GLfloat>()));

	return std::make_pair(output, stepCur);
}