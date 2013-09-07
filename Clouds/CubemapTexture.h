#pragma once

#include <string>

using std::string;

class CubemapTexture
{
public:
	CubemapTexture();
	~CubemapTexture();

	bool Load(const string& Directory,
		const string& PosXFilename,
		const string& NegXFilename,
		const string& PosYFilename,
		const string& NegYFilename,
		const string& PosZFilename,
		const string& NegZFilenam);

	void Bind(GLenum TextureUnit);

private:

	string m_fileNames[6];
	GLuint m_textureObj;
};