#include "stdafx.h"
#include "Cloud.h"

void Cloud::setMainShader(Shader* mainShader)
{
	mainShader->setContext(getContext());

	m_mainShader = mainShader;
	if (m_mainShader)
	{
		m_mainShader->use();
		m_uMVP      = mainShader->getUniform<glm::mat4>("MVP");
		m_uInvPV    = mainShader->getUniform<glm::mat4>("invPV");
		m_uWidth    = mainShader->getUniform<glm::vec3>("width");
		m_uCloudPos = mainShader->getUniform<glm::vec3>("cloudPos");
	}
}

void Cloud::renderImpl(){
	GET_CONTEXT();

	if (m_mainShader){
		m_uMVP.setValue(getProjectionView() * getModel());
		m_uWidth.setValue(m_width);
		m_uInvPV.setValue(glm::inverse(getProjectionView()));
		m_uCloudPos.setValue(getPosition());
	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);

	glDrawElements(	GL_QUADS, m_indicesSize, GL_UNSIGNED_INT, 0);
	
	glDisableVertexAttribArray(0);

	OPENGL_CHECK_FOR_ERRORS();
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
			GLfloat y = glm::sin( -glm::pi<GLfloat>() / 2.0f + glm::pi<GLfloat>() * r * R );
			const GLfloat x = glm::cos(2 * glm::pi<GLfloat>() * s * S) * glm::sin( glm::pi<GLfloat>() * r * R );
			const GLfloat z = glm::sin(2 * glm::pi<GLfloat>() * s * S) * glm::sin( glm::pi<GLfloat>() * r * R );

			if (y > 0.3f)  y = 0.3f;
			if (y < -0.3f) y = -0.3f + glm::gaussRand(0.2f, 0.2f);

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
