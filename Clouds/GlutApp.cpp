#include "stdafx.h"
#include "GlutApp.h"

#include <cassert>
#include "Utility.h"

GlutApp* GlutApp::self = nullptr;

GlutApp::GlutApp(int argc, _TCHAR* argv[], const char* appName)
{
	// if already created - assert
	assert(!self);
	self = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowPosition(960, 0);
	glutInitWindowSize(960, 1080);

	int windowId = 0;
	if (appName)
		windowId = glutCreateWindow(appName);
	else
		windowId = glutCreateWindow("Application");

	GLenum err = glewInit();
	fatalPrint(err != GLEW_OK, "Error with glut", err);

	mainWindowId = windowId;

	//GLuint VertexArrayID = 0;
	//glGenVertexArrays(1, &VertexArrayID);
	//glBindVertexArray(VertexArrayID);

	glDepthMask(GL_TRUE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glEnable (GL_DEPTH_TEST);
}

void GlutApp::exec()
{
	glutMainLoop();
}

GlutWindow GlutApp::getMainWindow() const
{
	return GlutWindow(mainWindowId);
}

GlutApp::~GlutApp(void)
{
	self = nullptr;
}
