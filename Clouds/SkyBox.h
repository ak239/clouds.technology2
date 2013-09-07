#pragma once

#include "RenderObject.h"
#include "Shader.h"
#include "UniformWrapper.h"
#include "CubemapTexture.h"

using std::string;

class SkyBox : public RenderObject
{
public:
	SkyBox():m_shader(0){}
	SkyBox(GLContext context):RenderObject(context), m_shader(0){}

	void setShader(Shader* shader);
	void loadTextures(const string& Directory,
				  const string& PosXFilename,
				  const string& NegXFilename,
				  const string& PosYFilename,
				  const string& NegYFilename,
				  const string& PosZFilename,
				  const string& NegZFilename);

private:
	void renderImpl();

	Shader* m_shader;

	UniformWrapper<glm::mat4> m_wvp;
	UniformWrapper<GLuint>    m_cubemapTextureID;

	CubemapTexture m_cubemap;

	GLuint m_VB;
	GLuint m_IB;
};

