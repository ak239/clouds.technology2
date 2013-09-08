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

void CloudsField::fill(GLuint cloudCount, const glm::vec3& leftBottomFar, const glm::vec3& rightTopNear, GLfloat coveragePercent)
{
	if (cloudCount < 1) return;

	GLfloat width  = rightTopNear.x - leftBottomFar.x;
	GLfloat height = rightTopNear.y - leftBottomFar.y;
	GLfloat length = rightTopNear.z - leftBottomFar.z;

	GLfloat total  = width * length;

	GLfloat avgWidth  = width  * glm::sqrt(coveragePercent) / glm::sqrt(cloudCount);
	GLfloat avgHeight = height * coveragePercent / cloudCount;
	GLfloat avgLength = length * glm::sqrt(coveragePercent) / glm::sqrt(cloudCount);

	m_clouds.resize(cloudCount, Cloud(getContext()));

	GLfloat current = 0.0f;

	for (GLuint i = 0; i < cloudCount - 1; ++i){
		GLfloat currentWidth  = glm::abs(normal(avgWidth,  0.1f));
		GLfloat currentLength = glm::abs(normal(avgLength, 0.1f));
		GLfloat currentHeight = glm::abs(normal(10.0f, 0.01f));

		current += currentLength * currentWidth;

		m_clouds[i].setWidth(glm::vec3(currentWidth, currentHeight, currentLength));
		LOG_DEBUG(boost::str(boost::format("Create cloud: (%f, %f, %f)") % currentWidth % currentLength % currentHeight));
	}

	GLfloat currentWidth  = std::abs(normal(avgWidth, 0.1f));
	GLfloat currentLength = (total * coveragePercent - current) / currentWidth;
	GLfloat currentHeight = std::abs(normal(10.0f, 0.01f));

	m_clouds[cloudCount - 1].setWidth(glm::vec3(currentWidth, currentHeight, currentLength));
	LOG_DEBUG(boost::str(boost::format("Create cloud: (%f, %f, %f)") % currentWidth % currentLength % currentHeight));

	GLfloat currentX = avgWidth / 2.0f;
	GLfloat currentZ = avgLength / 2.0f;

	GLfloat stepX = width  / std::sqrt(cloudCount);
	GLfloat stepZ = length / std::sqrt(cloudCount);

	for (GLuint i = 0; i < cloudCount; ++i){
		m_clouds[i].setPosition(glm::vec3(currentX + leftBottomFar.x, uniform(leftBottomFar.y, rightTopNear.y), currentZ + leftBottomFar.z));
		
		if (currentX + stepX > width)
			currentZ = fmod(currentZ + stepZ, length);

		currentX = fmod(currentX + stepX,  width);
	}

	m_mover.setFunc(CloudsMover(leftBottomFar, rightTopNear));

	for (GLuint i = 0; i < cloudCount; ++i){
		if (m_shader)
			m_clouds[i].setMainShader(m_shader);
		//m_clouds[i].addVarToBar();
		m_mover.addObject(&m_clouds[i]);
	}

	invalidateConstants();
	if (m_shader)
		m_uWindowSize.setValue(glm::vec2(static_cast<GLfloat>(glutGet(GLUT_WINDOW_WIDTH)), static_cast<GLfloat>(glutGet(GLUT_WINDOW_HEIGHT))));
	m_isMoveStop = false;
}

void CloudsField::renderImpl()
{
	if (!m_isMoveStop)
		m_mover.idleFunc();

	if (m_shader){
		m_shader->use();

		m_uSunColor.setValue(m_sunColor);
		m_uSkyColor.setValue(m_skyColor);
		m_uGroundColor.setValue(m_groundColor);

		m_uKts.setValue(m_kts);

		m_uCameraPos.setValue(getCameraPos());
		if (m_isSunMove)
			m_thetaSun = glm::mod(glutGet(GLUT_ELAPSED_TIME) / 10000.0f, glm::pi<GLfloat>() / 2.0f);
		m_uThetaSun.setValue(m_thetaSun);
		m_phiSun = glm::mod(glutGet(GLUT_ELAPSED_TIME) / 10000.0f, glm::pi<GLfloat>());
		m_phiSun = 0.0f;
		m_uPhiSun.setValue(m_phiSun);

		invalidateConstants();

		m_uSunConstants1.setValue(glm::vec3(CloudFuncs::b(m_thetaSun),
			CloudFuncs::c(m_thetaSun),
			CloudFuncs::r(m_thetaSun)));
		m_uSunConstants2.setValue(glm::vec2(CloudFuncs::t(m_thetaSun),
			CloudFuncs::kc(m_thetaSun)));
	}
	
	std::vector<std::pair<GLfloat, GLuint> > distIdx;
	glm::vec3 cameraPos = getCameraPos();
	for (std::size_t i = 0; i < m_clouds.size(); ++i)
		distIdx.push_back(std::make_pair(glm::dot(getCameraDir(), m_clouds[i].getPosition() - getCameraPos()), i));
	
	DistIdxSort pred;
	std::sort(distIdx.begin(), distIdx.end(), pred);

	for (std::size_t i = 0; i < m_clouds.size(); ++i){
		m_clouds[distIdx[i].second].setCameraPos(getCameraPos());
		m_clouds[distIdx[i].second].setProjectionView(getProjectionView());
		m_clouds[distIdx[i].second].setProjection(getProjection());
		m_clouds[distIdx[i].second].setCameraDir(getCameraDir());

		m_clouds[distIdx[i].second].render();
	}
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
	TwAddVarRW(bar, "ThetaSun",    TW_TYPE_FLOAT,   &m_thetaSun,  "");
	TwAddVarRW(bar, "PhiSun",      TW_TYPE_FLOAT,   &m_phiSun,    "");
	TwAddVarRW(bar, "kref",        TW_TYPE_FLOAT,   &CloudFuncs::kref,     "");
	TwAddVarRW(bar, "gammaref",    TW_TYPE_FLOAT,   &CloudFuncs::gammaRef, "");
	TwAddVarRW(bar, "zref",        TW_TYPE_FLOAT,   &CloudFuncs::zref,     "");
	TwAddVarRW(bar, "g",           TW_TYPE_FLOAT,   &CloudFuncs::g,        "");
	TwAddVarRW(bar, "IsMoveStop",  TW_TYPE_BOOL32,  &m_isMoveStop,  "");
	TwAddVarRW(bar, "IsSunMove",   TW_TYPE_BOOL32,  &m_isSunMove,   "");
	TwAddVarRW(bar, "SunColor",    TW_TYPE_COLOR3F, &m_sunColor[0], "");
	TwAddVarRW(bar, "SkyColor",    TW_TYPE_COLOR3F, &m_skyColor[0], "");
	TwAddVarRW(bar, "GroundColor", TW_TYPE_COLOR3F, &m_groundColor[0], "");
	TwAddSeparator(bar, "", "");
	TwAddVarRW(bar, "T3 kt",     TW_TYPE_FLOAT,   &m_kts[0],  "");
	TwAddVarRW(bar, "T2 kt",     TW_TYPE_FLOAT,   &m_kts[1],  "");
	TwAddVarRW(bar, "T1 kt",     TW_TYPE_FLOAT,   &m_kts[2],  "");
	TwAddVarRW(bar, "T0a kt",    TW_TYPE_FLOAT,   &m_kts[3],  "");
	TwAddVarRW(bar, "T0b kt",    TW_TYPE_FLOAT,   &m_kts[4],  "");
	TwAddVarRW(bar, "sky kt",    TW_TYPE_FLOAT,   &m_kts[5],  "");
	TwAddVarRW(bar, "ground kt", TW_TYPE_FLOAT,   &m_kts[6],  "");
	

	TwDefine(" 'Clouds Coverage' size='240 480' ");
}

void CloudsField::reshape(int width, int height)
{
	if (m_shader){
		m_shader->use();
		m_uWindowSize.setValue(glm::vec2(static_cast<GLfloat>(width), static_cast<GLfloat>(height)));
	}
}

void CloudsField::invalidateConstants()
{
	if (m_shader){
		m_shader->use();
		m_uKref.setValue(    CloudFuncs::kref);
		m_uGammaRef.setValue(CloudFuncs::gammaRef);
		m_uZRef.setValue(    CloudFuncs::zref);
		m_ug.setValue(       CloudFuncs::g);
	}
}