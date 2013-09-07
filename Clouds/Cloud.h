#pragma once

#include <vector>
#include "BillboardObject.h"
#include "Particle.h"

/*
CloudField constants:
	1. kref

Algorithm:
	1. Get view and sun angles
CloudField part:
	1. Calculate phase angle
	2. Calculate phase function value by phase angle
	3. Calculate phase function convolved with itself by phase angle
	4. Calculate peak phase function by phase angle
	5. Calculate dirac from ThetaView - ThetaSun
	6. Calculate dirac from PhiView - PhiSun
	7. Calculate cos(ThetaView) and cos(ThetaSun)
	8. Calculate b(ThetaSun), c(ThetaSun)

Cloud point part (shader part):
	1. Calculate height in point on surface
	2. Calculate width from point in view direction
	3. Calculate width from point in sun direction
	4. Calculate F(ThetaSun)
*/

class Cloud : public BillboardObject
{
public:
	Cloud():m_width(1.0f), m_bar(0){}
	Cloud(GLContext context):BillboardObject(context), m_width(1.0f), m_bar(0){}
	~Cloud(){if (m_bar) TwDeleteBar(m_bar); }

	void setMainShader(Shader* mainShader);

	void setParticleCount(GLuint _particleCount){ m_particleCount = _particleCount; }
	void setWidth(const glm::vec3& width){ m_width = width; generateEllipse(width, 64, 64); }

	void regenerateParticles();
	void setAngles(GLfloat thetaSun, GLfloat phiSun, GLfloat thetaView, GLfloat phiView, GLfloat Theta){
		m_thetaSun  = thetaSun;
		m_phiSun    = phiSun;
		m_thetaView = thetaView;
		m_phiView   = phiView;
		m_Theta     = Theta;
	}

	void addVarToBar();

private:
	void draw();

	Shader* m_mainShader;
	UniformWrapper<glm::mat4> m_mvp;
	UniformWrapper<glm::mat4> m_invMVP;
	UniformWrapper<glm::vec3> m_scale;

	UniformWrapper<glm::vec3> m_cloudPos;
	UniformWrapper<GLfloat>   m_uP;
	UniformWrapper<GLfloat>   m_uP2;

	glm::vec3 m_width;
	GLuint    m_particleCount;

	std::vector<Particle> m_particles;

	GLfloat m_thetaSun;
	GLfloat m_phiSun;
	GLfloat m_thetaView;
	GLfloat m_phiView;
	GLfloat m_Theta;

	TwBar* m_bar;

	void generateEllipse(glm::vec3 width, GLuint rings, GLuint sectors);
	GLuint m_vertexBuffer;
	GLuint m_elementBuffer;
	GLuint m_indicesSize;
};

