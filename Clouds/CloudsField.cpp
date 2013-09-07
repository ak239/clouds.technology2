#include "stdafx.h"
#include "CloudsField.h"
#include "CloudFuncs.h"

struct DistIdxSort
{
	bool operator()(const std::pair<GLfloat, GLuint>& fst,
		const std::pair<GLfloat, GLuint>& snd) const{
			return fst.first > snd.first;
	}
};

void CloudsField::fill(GLuint cloudCount, const glm::vec3& leftBottomFar, const glm::vec3& rightTopNear, GLfloat coveragePercent, GLuint particleCount)
{
	m_isStopped = true;

	if (cloudCount < 1) return;

	GLfloat width  = rightTopNear.x - leftBottomFar.x;
	GLfloat height = rightTopNear.y - leftBottomFar.y;
	GLfloat length = rightTopNear.z - leftBottomFar.z;

	GLfloat total  = width * length;

	GLfloat avgWidth  = width  * std::sqrt(coveragePercent) / std::sqrt(cloudCount);
	GLfloat avgHeight = height * coveragePercent / cloudCount;
	GLfloat avgLength = length * std::sqrt(coveragePercent) / std::sqrt(cloudCount);

	m_clouds.resize(cloudCount, Cloud(getContext()));

	GLfloat current = 0.0f;

	for (GLuint i = 0; i < cloudCount - 1; ++i){
		GLfloat currentWidth  = std::abs(normal(avgWidth,  0.1f));
		GLfloat currentLength = std::abs(normal(avgLength, 0.1f));
		GLfloat currentHeight = std::abs(normal(10.0f, 0.01f));

		current += currentLength * currentWidth;

		std::cout << currentWidth << "," << currentLength << "," << currentHeight << std::endl;

		m_clouds[i].setWidth(glm::vec3(currentWidth, currentHeight, currentLength));
	}

	GLfloat currentWidth  = std::abs(normal(avgWidth, 0.1f));
	GLfloat currentLength = (total * coveragePercent - current) / currentWidth;
	GLfloat currentHeight = std::abs(normal(10.0f, 0.01f));

	std::cout << currentWidth << "," << currentLength << "," << currentHeight << std::endl;

	m_clouds[cloudCount - 1].setWidth(glm::vec3(currentWidth, currentHeight, currentLength));
	//m_clouds[cloudCount - 1].setWidth(glm::vec3(1.0f, 1.0f, 2.0f));

	GLfloat currentX = avgWidth / 2.0f;
	GLfloat currentZ = avgLength / 2.0f;

	GLfloat stepX = width  / std::sqrt(cloudCount);
	GLfloat stepZ = length / std::sqrt(cloudCount);

	for (GLuint i = 0; i < cloudCount; ++i){
		m_clouds[i].setPosition(glm::vec3(currentX + leftBottomFar.x, uniform(leftBottomFar.y, rightTopNear.y), currentZ + leftBottomFar.z));
		//m_clouds[i].setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		
		std::cout << currentX << "," << currentZ << std::endl;

		if (currentX + stepX > width)
			currentZ = fmod(currentZ + stepZ, length);

		currentX = fmod(currentX + stepX,  width);
	}

	m_mover.setFunc(CloudsMover(leftBottomFar, rightTopNear));

	for (GLuint i = 0; i < cloudCount; ++i){
		m_clouds[i].setMainShader(m_shader);
		//m_clouds[i].setParticleCount(particleCount / cloudCount);
		//m_clouds[i].regenerateParticles();
		//m_clouds[i].addVarToBar();
		m_mover.addObject(&m_clouds[i]);
	}

	invalidateConstants();
	m_resolution.setValue(glm::vec2(static_cast<GLfloat>(glutGet(GLUT_WINDOW_WIDTH)), static_cast<GLfloat>(glutGet(GLUT_WINDOW_HEIGHT))));
	m_isStopped = false;
	m_isMoveStop = false;
}

void CloudsField::renderImpl()
{
	if (m_isStopped) return;

	//glDepthMask(GL_FALSE);
	//std::cout << glutGet(GLUT_WINDOW_WIDTH) << "," << glutGet(GLUT_WINDOW_HEIGHT) << std::endl;

	if (!m_isMoveStop)
		m_mover.idleFunc();

	//m_cameraDir = getCameraDir();
	//m_thetaView = glm::acos(m_cameraDir.z / glm::length(m_cameraDir));
	//m_phiView   = glm::atan2(m_cameraDir.y, m_cameraDir.x);

	//m_Theta     = CloudFuncs::angle(m_thetaSun, m_phiSun, m_thetaView, m_phiView);
	//m_currentP  = CloudFuncs::phase(m_Theta);
	//m_p2        = CloudFuncs::phaseByPhase(m_Theta);
	//m_pPeak     = CloudFuncs::phasePeak(m_Theta);

	//m_dirac1    = CloudFuncs::dirac(m_thetaView - m_thetaSun);
	//m_dirac2    = CloudFuncs::dirac(glm::abs(m_phiView - m_phiSun) - glm::pi<GLfloat>());

	m_shader->use();
	m_camDir.setValue(m_cameraDir);
	m_cameraPos.setValue(getCameraPos());
	m_uThetaSun.setValue(glm::pi<GLfloat>() / 2.5f);
	m_uPhiSun.setValue(m_phiSun);
	//m_uThetaView.setValue(m_thetaView);
	//m_uPhiView.setValue(m_phiView);
	//m_uP.setValue(m_currentP);
	//m_uP2.setValue(m_p2);
	m_uSunConstants1.setValue(glm::vec3(CloudFuncs::b(m_thetaSun),
		CloudFuncs::c(m_thetaSun),
		CloudFuncs::r(m_thetaSun)));
	m_uSunConstants2.setValue(glm::vec2(CloudFuncs::t(m_thetaSun),
		CloudFuncs::kc(m_thetaSun)));
	
	std::vector<std::pair<GLfloat, GLuint> > distIdx;
	glm::vec3 cameraPos = getCameraPos();
	for (std::size_t i = 0; i < m_clouds.size(); ++i)
		distIdx.push_back(std::make_pair(glm::dot(getCameraDir(), m_clouds[i].getPosition() - getCameraPos()), i));
	
	DistIdxSort pred;
	std::sort(distIdx.begin(), distIdx.end(), pred);

	//glDepthMask(GL_FALSE);
	for (std::size_t i = 0; i < m_clouds.size(); ++i){
		m_clouds[i].setAngles(m_thetaSun, m_phiSun, m_thetaView, m_phiView, m_Theta);

		m_clouds[distIdx[i].second].setCameraPos(getCameraPos());
		m_clouds[distIdx[i].second].setProjectionView(getProjectionView());
		m_clouds[distIdx[i].second].setProjection(getProjection());
		m_clouds[distIdx[i].second].setCameraDir(getCameraDir());

		m_clouds[distIdx[i].second].render();
	}
	//glDepthMask(GL_TRUE);

	//glDepthMask(GL_TRUE);
	//glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"some text");
}

GLfloat CloudsField::uniform(GLfloat min, GLfloat max)
{
	GLfloat delta = max - min;
	return static_cast<GLfloat>(rand()) * delta / RAND_MAX - min;
}

GLfloat CloudsField::normal(GLfloat mean, GLfloat stdev)
{
	GLfloat accum = 0.0f;
	for (int i = 0; i < 12; ++i) accum += uniform(0.0f, 1.0f);

	return (accum - 6.0f) * stdev + mean;
}

void CloudsField::addVarToBar(TwBar* bar)
{
	TwAddSeparator(bar, "", "");
	TwAddVarRO(bar, "ThetaSun",   TW_TYPE_FLOAT,  &m_thetaSun,  "");
	TwAddVarRO(bar, "PhiSun",     TW_TYPE_FLOAT,  &m_phiSun,    "");
	TwAddVarRO(bar, "ThetaView",  TW_TYPE_FLOAT,  &m_thetaView, "");
	TwAddVarRO(bar, "PhiView",    TW_TYPE_FLOAT,  &m_phiView,   "");
	TwAddVarRO(bar, "Theta",      TW_TYPE_FLOAT,  &m_Theta,     "");
	TwAddVarRO(bar, "Phase",      TW_TYPE_FLOAT,  &m_currentP,  "");
	TwAddVarRO(bar, "Phase2",     TW_TYPE_FLOAT,  &m_p2,        "");
	TwAddVarRO(bar, "PhasePeak",  TW_TYPE_FLOAT,  &m_pPeak,     "");
	TwAddVarRO(bar, "Dirac1",     TW_TYPE_FLOAT,  &m_dirac1,    "");
	TwAddVarRO(bar, "Dirac2",     TW_TYPE_FLOAT,  &m_dirac2,    "");
	TwAddVarRO(bar, "CameraX",    TW_TYPE_FLOAT,  &m_cameraDir.x, "");
	TwAddVarRO(bar, "CameraY",    TW_TYPE_FLOAT,  &m_cameraDir.y, "");
	TwAddVarRO(bar, "CameraZ",    TW_TYPE_FLOAT,  &m_cameraDir.z, "");
	TwAddVarRW(bar, "IsMoveStop", TW_TYPE_BOOL32, &m_isMoveStop,  "");

	TwDefine(" 'Clouds Coverage' size='240 480' ");
}

void CloudsField::reshape(int width, int height)
{
	m_shader->use();
	m_resolution.setValue(glm::vec2(static_cast<GLfloat>(width), static_cast<GLfloat>(height)));
}

void CloudsField::invalidateConstants()
{
	m_shader->use();
	m_uKref.setValue(    CloudFuncs::kref);
	m_uGammaRef.setValue(CloudFuncs::gammaRef);
	m_uZRef.setValue(    CloudFuncs::zref);
	m_ug.setValue(       CloudFuncs::g);
}