#pragma once

#include "Object.h"
#include "Shader.h"

class GLRenderObject : public Object
{
public:
	GLRenderObject():isRendering(true), m_shader(0), scale(1.0f), pos(0.0f), m_rotateVec(1.0f, 0.0f, 0.0f), m_angle(0.0f){}
	GLRenderObject(GLContext _context);
	virtual ~GLRenderObject(void);

	virtual void init(){}
	void render();
	void setIsRendering(bool _isRendering);
	bool getIsRendering() const;

	virtual Shader* getShader();
	void setShader(Shader* shader);

	void move(const glm::vec3& d);
	void setScale(const glm::vec3& _scale);
	void setColor(const glm::vec3& _color);
	void setRotate(const glm::vec3& _rotateVec, GLfloat _angle);

	glm::mat4 model();
	glm::vec3 color();
	glm::vec3 position();

	void setCamPos( glm::vec3 camPos){ m_camPos = camPos; }
	void setCamView(const glm::mat4& camView){ m_camView = camView; }

protected:
	glm::vec3 getCamPos() const{  return m_camPos; }
	glm::mat4 getCamView() const{ return m_camView; }

private:
	bool isRendering;

	virtual void renderImpl(){}
	Shader* m_shader;

	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec3 m_color;

	glm::vec3 m_rotateVec;
	GLfloat   m_angle;

	glm::vec3 m_camPos;
	glm::mat4 m_camView;
};

