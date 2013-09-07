#pragma once

#include "Object.h"
#include <vector>

class RenderObject : public Object
{
public:
	RenderObject(){}
	RenderObject(GLContext _context):Object(_context){}
	void render();
	virtual void reshape(int width, int height){}

	void setProjectionView(const glm::mat4& projectionView);
	void setCameraPos(const glm::vec3& camPos);

	void setPosition(const glm::vec3& position);
	const glm::vec3& getPosition() const;

	virtual glm::mat4 getModel();

	void setCameraDir(const glm::vec3& cameraDir)  { m_cameraDir = cameraDir; }
	void setCameraView(const glm::mat4& cameraView){ m_cameraView = cameraView; }
	void setProjection(const glm::mat4& projection){ m_projection = projection; }

protected:
	const glm::mat4& getProjectionView() const;
	const glm::vec3& getCameraPos() const;
	const glm::vec3& getCameraDir() const{  return m_cameraDir; }
	const glm::mat4& getCameraView() const{ return m_cameraView; }
	const glm::mat4& getProjection() const{ return m_projection; }

private:
	virtual void renderImpl() = 0;

	glm::mat4 m_projectionView;
	glm::mat4 m_projection;
	glm::vec3 m_camPos;
	glm::vec3 m_position;
	glm::vec3 m_cameraDir;
	glm::mat4 m_cameraView;
};

