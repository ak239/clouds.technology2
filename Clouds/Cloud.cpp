#include "stdafx.h"
#include "Cloud.h"

#include "ParticleField.h"
#include "CloudFuncs.h"

struct DistIdxSort
{
	bool operator()(const std::pair<GLfloat, GLuint>& fst,
		const std::pair<GLfloat, GLuint>& snd) const{
			return fst.first < snd.first;
	}
};

void Cloud::setMainShader(Shader* mainShader)
{
	mainShader->setContext(getContext());

	m_mainShader = mainShader;
	m_mainShader->use();
	m_mvp      = mainShader->getUniform<glm::mat4>("MVP");
	m_invMVP   = mainShader->getUniform<glm::mat4>("invMVP");
	m_scale    = mainShader->getUniform<glm::vec3>("scale");
	m_cloudPos = mainShader->getUniform<glm::vec3>("cloudPos");

	m_uP  = mainShader->getUniform<GLfloat>("P");
	m_uP2 = mainShader->getUniform<GLfloat>("P2");
}

void Cloud::draw(){
	GET_CONTEXT();

	//m_mainShader->use();

//	glm::vec3  camDir = getPosition() - getCameraPos();
//	m_thetaView = glm::acos(camDir.y / glm::length(camDir));
//	m_phiView   = glm::atan2(camDir.z, camDir.x);
//
//	GLfloat theta = CloudFuncs::angle(m_thetaSun, m_phiSun, m_thetaView, m_phiView);
//	GLfloat p     = CloudFuncs::phase(theta);
//	GLfloat p2    = CloudFuncs::phaseByPhase(theta);
//	GLfloat pPeak = CloudFuncs::phasePeak(theta);
//
//	m_uP.setValue(p);
//	m_uP2.setValue(p2);
//
//	glm::mat4 mvp = getProjectionView() * glm::scale(getModel(), m_width);
	m_mvp.setValue(getProjectionView() * getModel());
////	m_mvp.setValue(mvp);
	m_scale.setValue(m_width);
//	m_invMVP.setValue(glm::inverse(getProjectionView() * getModel()));
	m_invMVP.setValue(glm::inverse(getProjectionView()));
	m_cloudPos.setValue(getPosition());

//	glEnableVertexAttribArray(0);
	// Index buffer
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
 //
	//// Draw the triangles !
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);

//	glDrawArrays(GL_TRIANGLES, 0, 2700);
	glDrawElements(	GL_QUADS, m_indicesSize, GL_UNSIGNED_INT, 0);
	
	glDisableVertexAttribArray(0);
	OPENGL_CHECK_FOR_ERRORS();
//	glDisableVertexAttribArray(0);

	//std::cout << glewGetErrorString(glGetError()) << std::endl;
//	glutWireSphere(1.0, 64, 64);
	//glutSolidSphere(1.0, 64, 64);
	//glutSolidDodecahedron();
	//gluSphere(
	
	//GLfloat 
	//glm::dot(camera.view, object.worldpos - camera.worldpos))
	//std::vector<std::pair<GLfloat, GLuint> > distIdx;
	//for (GLuint i = 0; i < m_particles.size(); ++i)
	//	distIdx.push_back(std::make_pair(glm::dot(getCameraDir() ,m_particles[i].getOffset() + getPosition() - getCameraPos()), i));

	//DistIdxSort pred;
	//std::sort(distIdx.begin(), distIdx.end(), pred);

	//for (GLuint i = 0; i < m_particles.size(); ++i){
	//	m_particles[distIdx[i].second].setCameraDir(getCameraDir());
	//	m_particles[distIdx[i].second].setProjectionView(getProjectionView());
	//	m_particles[distIdx[i].second].setCenter(getPosition());
	//	m_particles[distIdx[i].second].render();
	//}

}

void Cloud::regenerateParticles()
{
	m_particles.swap(std::vector<Particle>());

	Shader* particleShader  = Particle::LoadShader(getContext());
	GLuint  particleVertex  = Particle::GetVertexBuffer(getContext());
	GLuint  particleTexture = Particle::GetTextureID(getContext());

	ParticleField field(m_width[0], m_width[1], m_width[2]);
	std::pair<std::vector<ParticleEllipse>, GLfloat> genResult = field.generate(m_particleCount);
	const std::vector<ParticleEllipse>& ellipses = genResult.first;

	for (GLuint i = 0; i < ellipses.size(); ++i)
	{
		GLfloat curCos   = ellipses[i].m_cos;
		GLfloat curSin   = glm::sqrt(1 - curCos * curCos);
		GLfloat curAngle = ellipses[i].m_firstAngle;

		while (curAngle < 2.0 * glm::pi<GLfloat>()){
			glm::vec3 offset;
			offset.x = m_width[0] * glm::cos(curAngle) * curSin;
			offset.y = m_width[1] * glm::sin(curAngle) * curSin;
			offset.z = m_width[2] * curCos;

			m_particles.push_back(Particle(getContext(), particleShader,
				particleVertex, particleTexture));

			m_particles.back().setOffset(offset);
			m_particles.back().setWidth(genResult.second);

			curAngle += ellipses[i].m_stepAngle;
		}
	}
}

void Cloud::addVarToBar()
{
	m_bar = TwNewBar(boost::str(boost::format("Cloud %f:%f:%f") % m_width.x % m_width.y % m_width.z).c_str());

	TwAddVarRO(m_bar, "widthX",  TW_TYPE_FLOAT, &m_width.x, "");
	TwAddVarRO(m_bar, "widthY",  TW_TYPE_FLOAT, &m_width.y, "");
	TwAddVarRO(m_bar, "widthZ",  TW_TYPE_FLOAT, &m_width.z, "");
	
}

void Cloud::generateEllipse(glm::vec3 width, GLuint rings, GLuint sectors)
{
	GET_CONTEXT();
	const GLfloat R = 1.0f/(rings - 1.0f);
	const GLfloat S = 1.0f/(sectors - 1.0f);

	std::vector<glm::vec3> vertices(rings * sectors);

	std::vector<glm::vec3>::iterator v = vertices.begin();
	
	for(GLint r = 0; r < rings; r++) 
		for(GLint s = 0; s < sectors; s++) 
		{
			const GLfloat y = glm::sin( -glm::pi<GLfloat>() / 2.0f + glm::pi<GLfloat>() * r * R );
			const GLfloat x = glm::cos(2 * glm::pi<GLfloat>() * s * S) * glm::sin( glm::pi<GLfloat>() * r * R );
			const GLfloat z = glm::sin(2 * glm::pi<GLfloat>() * s * S) * glm::sin( glm::pi<GLfloat>() * r * R );

			*v++ = glm::vec3( x * width.x, y * width.y, z * width.z);
		}

	std::vector<GLuint> indices(rings * sectors * 4);
	std::vector<GLuint>::iterator i = indices.begin();
	for(GLint r = 0; r < rings - 1; r++) 
		for(GLint s = 0; s < sectors - 1; s++) 
		{
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s+1);
			*i++ = (r+1) * sectors + (s+1);
			*i++ = (r+1) * sectors + s;
		}


	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	OPENGL_CHECK_FOR_ERRORS();

	m_indicesSize = indices.size();
}
