#include "stdafx.h"
#include "BillboardObject.h"

const GLfloat FarDistance  = 10.0f;
const GLfloat NearDistance = 7.0f;

// The fullscreen quad's FBO
static const GLfloat g_quad_vertex_buffer_data[] = { 
	-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
};

BillboardObject::BillboardObject(void):m_billboardShader(0), m_isNowBillboard(false)
{
}

BillboardObject::BillboardObject(GLContext context):RenderObject(context),
	m_billboardShader(0), m_isNowBillboard(false)
{
}

void BillboardObject::setBillboardShader(Shader* billboardShader)
{
	m_billboardShader = billboardShader;

	m_texId        = m_billboardShader->getUniform<GLuint>("renderedTexture");
	m_billboardMVP = m_billboardShader->getUniform<glm::mat4>("MVP");

	glGenBuffers(1, &m_quadVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
}

void BillboardObject::renderImpl()
{
	draw(); return;

	GLfloat dist = glm::distance(getCameraPos(), getPosition());
	if (dist > FarDistance && !m_isNowBillboard){
		createBillboard();
		m_isNowBillboard = true;
	}
	
	if (dist < NearDistance && m_isNowBillboard)
		m_isNowBillboard = false;

	if (m_isNowBillboard)
		drawBillboard();
	else
		draw();
}

void BillboardObject::drawBillboard()
{
	GET_CONTEXT();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_billboardShader->use();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
	
	// Set our "renderedTexture" sampler to user Texture Unit 0
	m_texId.setValue(0);

	glm::vec3 cameraDirection = getCameraDir();
	glm::vec3 planeNormal     = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 axis  = glm::normalize(glm::cross(planeNormal, cameraDirection));
	GLfloat   angle = glm::acos(glm::dot(planeNormal, cameraDirection)) * 180.0f / glm::pi<float>();

	m_billboardMVP.setValue(getProjectionView() * (glm::translate(getPosition()) * glm::rotate(angle, axis)));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVertexbuffer);

	glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

	glDisableVertexAttribArray(0);
}

void BillboardObject::createBillboard()
{
	GET_CONTEXT();

	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// The texture we're going to render to
	glGenTextures(1, &m_renderedTexture);
 
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
 
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 1024, 768, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
 
	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// The depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderedTexture, 0);
 
	// Set the list of draw buffers.
	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers


	// Render to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	//glViewport(0,0,1024,768); // Render on the whole framebuffer, complete from the lower left corner to the upper right

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw();
}
