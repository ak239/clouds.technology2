#pragma once

#include "Cloud.h"
#include "RenderObject.h"
#include "Mover.h"

#include <vector>
#include <cmath>

struct CloudsMover{
public:
	CloudsMover(){}
	CloudsMover(const glm::vec3& leftBottomFar, const glm::vec3& rightTopNear):
		m_leftBottomFar(leftBottomFar), m_rightTopNear(rightTopNear)
	{
	}

	glm::vec3 operator()(GLfloat time){
		GLfloat dx = std::fmod(time / 400.0f, m_rightTopNear.x - m_leftBottomFar.x);
		return glm::vec3(dx + m_rightTopNear.x, 0.0f, 0.0f);
	}

private:
	glm::vec3 m_leftBottomFar;
	glm::vec3 m_rightTopNear;
};

class CloudsField : public RenderObject
{
public:
	CloudsField() : m_isStopped(true), m_shader(0), m_thetaSun(glm::pi<GLfloat>() / 8.0f), m_phiSun(0.0f), m_isMoveStop(true){}
	CloudsField(GLContext context) : RenderObject(context), m_isStopped(true), m_shader(0), m_thetaSun(glm::pi<GLfloat>() / 8.0f), m_phiSun(0.0f), m_isMoveStop(true){}

	void fill(GLuint cloudCount, const glm::vec3& leftBottomFar, const glm::vec3& rightTopNear, GLfloat coveragePercent,
		GLuint particleCount = 1000);

	void setMainShader(Shader* mainShader)
	{ 
		m_shader     = mainShader; 
		m_resolution = mainShader->getUniform<glm::vec2>("windowSize");
		m_camDir     = mainShader->getUniform<glm::vec3>("cameraDir");
		m_cameraPos  = mainShader->getUniform<glm::vec3>("cameraPos");

		m_uKref      = mainShader->getUniform<GLfloat>("kref");
		m_uThetaSun  = mainShader->getUniform<GLfloat>("thetaSun");
		m_uPhiSun    = mainShader->getUniform<GLfloat>("phiSun");
		//m_uThetaView = mainShader->getUniform<GLfloat>("thetaView");
		//m_uPhiView   = mainShader->getUniform<GLfloat>("phiView");
		//m_uP         = mainShader->getUniform<GLfloat>("P");
		//m_uP2        = mainShader->getUniform<GLfloat>("P2");
		m_uGammaRef  = mainShader->getUniform<GLfloat>("gammaref");
		m_uZRef      = mainShader->getUniform<GLfloat>("zref");
		m_uSunConstants1 = mainShader->getUniform<glm::vec3>("sunConstants1");
		m_uSunConstants2 = mainShader->getUniform<glm::vec2>("sunConstants2");
		m_ug         = mainShader->getUniform<GLfloat>("g");

//		for (int i = 0; i < m_clouds.size(); ++i)
//			m_clouds[i].setMainShader(mainShader);
	}

	void addVarToBar(TwBar* bar);

	void reshape(int width, int height);
	void invalidateConstants();

private:
	void renderImpl();

	std::vector<Cloud> m_clouds;

	static GLfloat uniform(GLfloat min, GLfloat max);
	static GLfloat normal(GLfloat mean, GLfloat stdev);

	bool m_isStopped;

	Shader*            m_shader;
	Mover<CloudsMover> m_mover;

	GLfloat m_thetaSun;
	GLfloat m_phiSun;

	UniformWrapper<glm::vec2> m_resolution;
	UniformWrapper<glm::vec3> m_camDir;
	UniformWrapper<glm::vec3> m_cameraPos;

	UniformWrapper<GLfloat>   m_uKref;
	UniformWrapper<GLfloat>   m_uThetaSun;
	UniformWrapper<GLfloat>   m_uPhiSun;
	UniformWrapper<GLfloat>   m_uThetaView;
	UniformWrapper<GLfloat>   m_uPhiView;
	UniformWrapper<GLfloat>   m_uP;
	UniformWrapper<GLfloat>   m_uP2;
	UniformWrapper<GLfloat>   m_uGammaRef;
	UniformWrapper<GLfloat>   m_uZRef;
	UniformWrapper<glm::vec3> m_uSunConstants1;
	UniformWrapper<glm::vec2> m_uSunConstants2;
	UniformWrapper<GLfloat>   m_ug;

	mutable GLfloat   m_currentP;
	mutable GLfloat   m_thetaView;
	mutable GLfloat   m_phiView;
	mutable GLfloat   m_Theta;
	mutable GLfloat   m_p2;
	mutable GLfloat   m_pPeak;
	mutable GLfloat   m_dirac1;
	mutable GLfloat   m_dirac2;
	mutable glm::vec3 m_cameraDir;

	bool m_isMoveStop;	
};

