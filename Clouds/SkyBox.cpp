#include "stdafx.h"
#include "SkyBox.h"

struct Vertex{
	glm::vec3 xys;
	glm::vec2 uv;
	glm::vec3 norm;
};

const Vertex gVertices[24] =
{
	{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
	{glm::vec3(-1.0f, -1.0f, +1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }, 
	{glm::vec3(+1.0f, -1.0f, +1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }, 
	{glm::vec3(+1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
		
	{glm::vec3(-1.0f, +1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f ) }, 
	{glm::vec3(-1.0f, +1.0f, +1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f ) }, 
	{glm::vec3(+1.0f, +1.0f, +1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f ) }, 
	{glm::vec3(+1.0f, +1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f ) },
		
	{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) }, 
	{glm::vec3(-1.0f, +1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) }, 
	{glm::vec3(+1.0f, +1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) }, 
	{glm::vec3(+1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
		
	{glm::vec3(-1.0f, -1.0f, +1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f ) }, 
	{glm::vec3(-1.0f, +1.0f, +1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f ) }, 
	{glm::vec3(+1.0f, +1.0f, +1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f ) }, 
	{glm::vec3(+1.0f, -1.0f, +1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f ) },
		
	{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }, 
	{glm::vec3(-1.0f, -1.0f, +1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }, 
	{glm::vec3(-1.0f, +1.0f, +1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) }, 
	{glm::vec3(-1.0f, +1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) },

	{glm::vec3(+1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f ) }, 
	{glm::vec3(+1.0f, -1.0f, +1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f ) }, 
	{glm::vec3(+1.0f, +1.0f, +1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f ) }, 
	{glm::vec3(+1.0f, +1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f ) } 
};

const GLuint gIndices[36] =
{
	0,2,1, 0,3,2, 
	4,5,6, 4,6,7, 
	8,9,10, 8,10,11, 
	12,15,14, 12,14,13, 
	16,17,18, 16,18,19, 
	20,23,22, 20,22,21
};

void SkyBox::setShader(Shader* shader)
{
	GET_CONTEXT();

	m_shader = shader;

	m_wvp              = shader->getUniform<glm::mat4>("gWVP");
	m_cubemapTextureID = shader->getUniform<GLuint>("gCubemapTexture");
}

void SkyBox::loadTextures(const string& Directory,
				const string& PosXFilename,
				const string& NegXFilename,
				const string& PosYFilename,
				const string& NegYFilename,
				const string& PosZFilename,
				const string& NegZFilename)
{
	GET_CONTEXT();
	m_cubemapTextureID.setValue(0);

	m_cubemap.Load(Directory, PosXFilename, NegXFilename,
		PosYFilename, NegYFilename, PosZFilename, NegZFilename);

	glGenBuffers(1, &m_VB);
	glBindBuffer(GL_ARRAY_BUFFER, m_VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertices), &gVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndices), &gIndices[0], GL_STATIC_DRAW);
}

void SkyBox::renderImpl()
{
	GET_CONTEXT();

	m_shader->use();

	GLint OldCullFaceMode = 0;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode = 0;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);
	glDepthFunc(GL_LEQUAL);

	//std::cout << getCameraPos()[0] << "," << getCameraPos()[1] << "," << getCameraPos()[0] << std::endl;
	setPosition(getCameraPos());

	glm::mat4 MVP = getProjectionView() * (glm::rotate(180.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * getModel());

	m_wvp.setValue(MVP);
			
	m_cubemap.Bind(GL_TEXTURE0);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, m_VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 20);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IB);

	glDrawElements(GL_TRIANGLES, sizeof(gIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glCullFace(OldCullFaceMode);        
	glDepthFunc(OldDepthFuncMode);
}
