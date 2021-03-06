#include "stdafx.h"
#include "Scene.h"

void Scene::reshape(int width, int height)
{
	GET_CONTEXT();

	glViewport(0,0,width,height);

	for (std::size_t i = 0; i < m_objects.size(); ++i)
		m_objects[i]->reshape(width, height);

	TwWindowSize(width, height);
	glutPostRedisplay();
}

void Scene::display()
{
	GET_CONTEXT();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	TwWindowSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	GLfloat aspect = static_cast<GLfloat>(glutGet(GLUT_WINDOW_WIDTH))/glutGet(GLUT_WINDOW_HEIGHT);
	glm::mat4 projection = glm::perspective(45.0f, aspect, 0.1f, 500.0f);
	
	glm::mat4 projectionView;

	if (m_camera)
		projectionView = projection * m_camera->view();
	else
		projectionView = projection;

	for (std::size_t i = 0; i < m_objects.size(); ++i)
	{
		m_objects[i]->setCameraPos(m_camera->getPos());
		m_objects[i]->setProjectionView(projectionView);
		m_objects[i]->setProjection(projection);
		m_objects[i]->setCameraDir(m_camera->getDir());
		m_objects[i]->setCameraView(m_camera->view());
		m_objects[i]->render();
	}

	TwDraw();
	glutSwapBuffers();
}

void Scene::addObject(RenderObject* object)
{
	object->setContext(getContext());
	m_objects.push_back(object);
}

void Scene::setCamera(Camera* camera)
{
	m_camera = camera;
}
