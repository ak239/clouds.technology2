#include "stdafx.h"
#include "GlutApp.h"

#include <cassert>

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
	if (err != GLEW_OK) LOG_ERROR(boost::str(boost::format("Error with glut inititalization: %X") % err));

	mainWindowId = windowId;

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glEnable (GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
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
