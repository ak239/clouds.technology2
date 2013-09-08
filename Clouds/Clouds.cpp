// Clouds.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <cmath>

#include "GlutApp.h"
#include "Logger.h"
#include "UniformWrapper.h"
#include "Camera.h"
#include "MouseCamera.h"
#include "Scene.h"
#include "Cloud.h"
#include "SkyBox.h"
#include "Mover.h"
#include "CloudsField.h"

Camera camera;
MouseCamera mouseCamera;
CloudsField cloudsField;

typedef glm::vec3 (*Func)(GLfloat);

Mover<Func> moverTest;

GLfloat AdditionAngle = 0.0f;

GLfloat HorAngle   = glm::pi<GLfloat>();
GLfloat VerAngle   = 0.0f;
GLfloat MouseSpeed = 0.0001f;
GLint   LastTime   = 0;
GLint   LastX      = 0;
GLint   LastY      = 0;
bool    IsCameraMove = true;

glm::vec3 LeftBottomFar  = glm::vec3(-50.0f, -25.0f, -50.0f);
glm::vec3 RightTopNear   = glm::vec3(50.0f,  -3.0f, 50.0f);
GLuint    CloudsCount    = 6;
GLfloat   CloudsCoverage = 0.5;

Shader* CloudShader = 0;
GlutWindow* MainWindow = 0;

void keyboardFunc(unsigned char key, int x, int y);
void motionFunc(int x, int y);
void idleFunc();
void timerFunc(int value);
void reshapeFunc(int width, int height);
void closeFunc();

void TW_CALL initCloudsField(void *clientData);
void TW_CALL reloadShader(void *clientData);

glm::vec3 SimpleSinFunc(GLfloat time);
glm::vec3 SimpleLinkedFunc(GLfloat time);

int _tmain(int argc, _TCHAR* argv[])
{
	Logger::instance().setStream(std::cout);

	GlutApp app(argc, argv, "Clouds App");

	GlutWindow wnd = app.getMainWindow();
	MainWindow = &wnd;

	TwInit(TW_OPENGL_CORE, NULL);
	TwBar *myBar;
	myBar = TwNewBar("Clouds Coverage");

	TwAddVarRW(myBar, "CloudsCount",    TW_TYPE_UINT16, &CloudsCount, "");
	TwAddVarRW(myBar, "CloudsCoverage", TW_TYPE_FLOAT,  &CloudsCoverage, "");
	TwAddSeparator(myBar, "Left Bottom Far", "");
	TwAddVarRW(myBar, "X1", TW_TYPE_FLOAT, &LeftBottomFar[0], "");
	TwAddVarRW(myBar, "Y1", TW_TYPE_FLOAT, &LeftBottomFar[1], "");
	TwAddVarRW(myBar, "Z1", TW_TYPE_FLOAT, &LeftBottomFar[2], "");
	TwAddSeparator(myBar, "Right Top Near", "");
	TwAddVarRW(myBar, "X2", TW_TYPE_FLOAT, &RightTopNear[0], "");
	TwAddVarRW(myBar, "Y2", TW_TYPE_FLOAT, &RightTopNear[1], "");
	TwAddVarRW(myBar, "Z2", TW_TYPE_FLOAT, &RightTopNear[2], "");
	
	TwAddButton(myBar, "Apply", &initCloudsField, 0, "");
	TwAddButton(myBar, "Reload Shader", &reloadShader, 0, "");

	mouseCamera.setPos(glm::vec3(4, 0.0f, 0));
	mouseCamera.setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	mouseCamera.setHead(glm::vec3(0.0f, 1.0f, 0.0f));

	Scene scene;
	scene.setCamera(&mouseCamera);
	wnd.setScene(&scene);

	Shader skyboxShader(wnd.getContext());
	skyboxShader.loadFromFile("skybox", Shader::Vertex | Shader::Fragment);
	skyboxShader.link();

	cloudsField.setContext(wnd.getContext());
	cloudsField.addVarToBar(myBar);

	reloadShader(0);

	SkyBox skybox(wnd.getContext());
	skybox.setShader(&skyboxShader);
	skybox.loadTextures(".\\media\\",
		"posx.dds",
		"negx.dds",
		"posy.dds",
		"negy.dds",
		"posz.dds",
		"negz.dds");
	scene.addObject(&skybox);

	scene.addObject(&cloudsField);

	initCloudsField(0);

	glutKeyboardFunc(&keyboardFunc);

	glutMotionFunc(&motionFunc);

	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT); 
	glutIdleFunc(&idleFunc);
	glutTimerFunc(33, &timerFunc, 0);

	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	glutCloseFunc(&closeFunc);

	TwGLUTModifiersFunc(glutGetModifiers);
	app.exec();
}

void keyboardFunc(unsigned char key, int x, int y)
{
	switch(key){
	case 'y':
		camera.move(glm::vec3(1.0, 0.0, 0.0));
		break;
	case 'u':
		camera.move(glm::vec3(-1.0, 0.0, 0.0));
		break;
	case 'h':
		camera.move(glm::vec3(0.0, 1.0, 0.0));
		break;
	case 'j':
		camera.move(glm::vec3(0.0, -1.0, 0.0));
		break;
	case 'n':
		camera.move(glm::vec3(0.0, 0.0, 1.0));
		break;
	case 'm':
		camera.move(glm::vec3(0.0, 0.0, -1.0));
		break;
	case 'q':
		mouseCamera.moveByDir(0.2f);
		break;
	case 'a':
		mouseCamera.moveByDir(-0.2f);
		break;

	case 'p':
		glutExit();
		break;

	case 'o':
		IsCameraMove = !IsCameraMove;
		break;
	}

	glm::vec3 cameraDirection = mouseCamera.getDir();
	glm::vec3 planeNormal     = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 axis  = glm::normalize(glm::cross(planeNormal, cameraDirection));
	GLfloat   angle = glm::acos(glm::dot(planeNormal, cameraDirection));

	TwEventKeyboardGLUT(key, x, y);
}

void motionFunc(int x, int y)
{
	int halfXWindow = glutGet(GLUT_WINDOW_WIDTH) / 2;
	int halfYWindow = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	GLint time = glutGet(GLUT_ELAPSED_TIME);
	GLint deltaTime = time - LastTime;
	if (deltaTime > 128 && IsCameraMove)
		LastTime = time;

	if (deltaTime > 0 && IsCameraMove){
		LastTime = time;

		GLfloat deltaHor = MouseSpeed * deltaTime * float(halfXWindow - x );
		GLfloat deltaVer = MouseSpeed * deltaTime * float(halfYWindow - y );

		if (std::abs(deltaHor) > 0.03f) deltaHor = 0.03f * (deltaHor > 0.0 ? 1 : -1);
		if (std::abs(deltaVer) > 0.03f) deltaVer = 0.03f * (deltaVer > 0.0 ? 1 : -1);

		HorAngle += deltaHor;
		VerAngle += deltaVer;

		mouseCamera.setAngles(HorAngle, VerAngle);

		glutWarpPointer(halfXWindow, halfYWindow);
	}

	TwEventMouseMotionGLUT(x, y);
}

void timerFunc(int value)
{
	glutPostRedisplay();
	glutTimerFunc(33, &timerFunc, 0);

	moverTest.idleFunc();
}

void idleFunc()
{
//	CloudsMover.idleFunc();
}

void reshapeFunc(int width, int height)
{
	TwWindowSize(width, height);
}

void closeFunc()
{
	TwTerminate();
}

glm::vec3 SimpleSinFunc(GLfloat time)
{
	return glm::vec3(std::sin(time / 200.0f), 0.0f, 0.0f);
}

glm::vec3 SimpleLinkedFunc(GLfloat time)
{	
	GLfloat val = std::fmod(time / 200.0f, 10.0f) - 5.0f;
	return glm::vec3(0.0f, 0.0f, val);
}

void TW_CALL initCloudsField(void *clientData)
{
	cloudsField.fill(CloudsCount, LeftBottomFar, RightTopNear, CloudsCoverage);
}

void TW_CALL reloadShader(void *clientData)
{
	if (CloudShader) delete CloudShader;
	CloudShader = new Shader(MainWindow->getContext());

	CloudShader->loadFromFile("cloud", Shader::Vertex | Shader::Fragment);
	if (CloudShader->link())
		cloudsField.setMainShader(CloudShader);
}