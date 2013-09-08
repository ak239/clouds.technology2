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
	CloudsField() : m_shader(0), m_thetaSun(glm::pi<GLfloat>() / 8.0f), m_phiSun(0.0f), m_isMoveStop(true), 
		m_sunColor(227.0f / 255.0f, 168.0f / 255.0f, 87.0f / 255.0f),
		m_skyColor(0.0f, 191.0f / 255.0f, 1.0f),
		m_groundColor(0.0f, 196.0f / 255.0f, 58.0f / 255.0f),
		m_isSunMove(true)
	{
		for (int i = 0; i < sizeof(m_kts) / sizeof(GLfloat); ++i) m_kts[i] = 1.0f;
	}
	CloudsField(GLContext context) : RenderObject(context), m_shader(0), m_thetaSun(glm::pi<GLfloat>() / 8.0f), m_phiSun(0.0f), m_isMoveStop(true), 
		m_sunColor(227.0f / 255.0f, 168.0f / 255.0f, 87.0f / 255.0f),
		m_skyColor(0.0f, 191.0f / 255.0f, 1.0f),
		m_groundColor(0.0f, 196.0f / 255.0f, 58.0f / 255.0f),
		m_isSunMove(true)
	{
		for (int i = 0; i < sizeof(m_kts) / sizeof(GLfloat); ++i) m_kts[i] = 1.0f;
	}

	void fill(GLuint cloudCount, const glm::vec3& leftBottomFar, const glm::vec3& rightTopNear, GLfloat coveragePercent);

	void setMainShader(Shader* mainShader)
	{ 
		m_shader      = mainShader; 

		m_uWindowSize = mainShader->getUniform<glm::vec2>("windowSize");
		m_uCameraPos  = mainShader->getUniform<glm::vec3>("cameraPos");

		m_uKref      = mainShader->getUniform<GLfloat>("kref");
		m_uThetaSun  = mainShader->getUniform<GLfloat>("thetaSun");
		m_uPhiSun    = mainShader->getUniform<GLfloat>("phiSun");
		m_uGammaRef  = mainShader->getUniform<GLfloat>("gammaref");
		m_uZRef      = mainShader->getUniform<GLfloat>("zref");
		m_uSunConstants1 = mainShader->getUniform<glm::vec3>("sunConstants1");
		m_uSunConstants2 = mainShader->getUniform<glm::vec2>("sunConstants2");
		m_ug         = mainShader->getUniform<GLfloat>("g");

		m_uSunColor    = mainShader->getUniform<glm::vec3>("sunColor");
		m_uSkyColor    = mainShader->getUniform<glm::vec3>("skyColor");
		m_uGroundColor = mainShader->getUniform<glm::vec3>("groundColor");

		m_uKts = mainShader->getUniform<GLfloat[7]>("kts");
	}

	void addVarToBar(TwBar* bar);

	void reshape(int width, int height);
	void invalidateConstants();

private:
	void renderImpl();

	std::vector<Cloud> m_clouds;

	static GLfloat uniform(GLfloat min, GLfloat max);
	static GLfloat normal(GLfloat mean, GLfloat stdev);

	Shader*            m_shader;
	Mover<CloudsMover> m_mover;

	GLfloat m_thetaSun;
	GLfloat m_phiSun;

	UniformWrapper<glm::vec2> m_uWindowSize;
	UniformWrapper<glm::vec3> m_uCameraPos;

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

	glm::vec3                 m_sunColor;
	UniformWrapper<glm::vec3> m_uSunColor;
	glm::vec3                 m_skyColor;
	UniformWrapper<glm::vec3> m_uSkyColor;
	glm::vec3                 m_groundColor;
	UniformWrapper<glm::vec3> m_uGroundColor;

	GLfloat m_kts[7];
	UniformWrapper<GLfloat[7]> m_uKts;

	bool m_isMoveStop;	
	bool m_isSunMove;
};

