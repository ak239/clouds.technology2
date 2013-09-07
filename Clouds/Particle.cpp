#include "stdafx.h"
#include "Particle.h"
#include "nvImage.h"

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
   1.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
   -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
   -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
   1.0f,   1.0f, 0.0f,  1.0f, 1.0f,
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f//,
   //0.5f,   0.5f, 0.0f, 1.0f, 1.0f
};

Shader* Particle::LoadShader(const GLContext& context)
{
	static Shader* shader = nullptr;
	if (shader == nullptr)
		shader = new Shader(context);
	else
		return shader;
	shader->loadFromFile("particle", Shader::Fragment | Shader::Vertex);
	shader->link();
	return shader;
}

GLuint  Particle::GetVertexBuffer(const GLContext& context)
{
	GLContextGetter getter(context);

	static GLuint vertexBuffer = -1;
	if (vertexBuffer != -1) return vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	return vertexBuffer;
}

GLuint  Particle::GetTextureID(const GLContext& context)
{
	GLContextGetter getter(context);

	static GLuint tmpTexId = -1;
	if (tmpTexId != -1)
		return tmpTexId;

	nv::Image fImage;
	fImage.loadImageFromFile(".\\media\\metaball.dds");

	glGenTextures( 1, &tmpTexId);
	glBindTexture( GL_TEXTURE_2D, tmpTexId);

	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	int w = fImage.getWidth();
	int h = fImage.getHeight();

	for (int i = 0; i < fImage.getMipLevels(); ++i){
		glTexImage2D( GL_TEXTURE_2D, i, fImage.getInternalFormat(), w, h, 0, fImage.getFormat(), fImage.getType(), fImage.getLevel(i));

        w = ( w > 1) ? w >> 1 : 1;
        h = ( h > 1) ? h >> 1 : 1;
	}
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

	return tmpTexId;
}

Particle::Particle(GLContext context, Shader* shader, GLuint vertexBuffer, GLuint texId): RenderObject(context), m_shader(shader),m_vertexBuffer(vertexBuffer), m_texId(texId)
{
	GET_CONTEXT();
	if (m_shader){
		m_MVP         = m_shader->getUniform<glm::mat4>("gWVP");
		//m_rightNormal = m_shader->getUniform<glm::vec4>("rightNormal");
		//m_upNormal    = m_shader->getUniform<glm::vec4>("upNormal");
		m_shader->getUniform<GLuint>("gSampler").setValue(0);
	}
}


void Particle::init()
{

}

void Particle::renderImpl()
{
	m_shader->use();

	glm::vec3 cameraDirection = getCameraDir();
	glm::vec3 planeNormal     = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 axis  = glm::normalize(glm::cross(planeNormal, cameraDirection));
	GLfloat   angle = glm::acos(glm::dot(planeNormal, cameraDirection)) * 180.0f / glm::pi<float>();

	setPosition(m_center + m_offset);

	m_MVP.setValue(getProjectionView() * (getModel() * glm::scale(glm::vec3(m_width)) * glm::rotate(angle, axis)));
	//const glm::mat4& cameraView = getCameraView();
	//m_rightNormal.setValue(glm::normalize(glm::vec4(cameraView[0][0], cameraView[1][0], cameraView[2][0], 0.0f)));
	//m_upNormal.setValue(   glm::normalize(glm::vec4(cameraView[0][1], cameraView[1][1], cameraView[2][1], 0.0f)));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE0, m_texId);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   sizeof(GLfloat) * 5,                  // stride
	   (void*)0            // array buffer offset
	);
 
	glVertexAttribPointer(
	   1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   2,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   sizeof(GLfloat) * 5,                  // stride
	   (void*)(sizeof(GLfloat) * 3)  // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
 
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}
