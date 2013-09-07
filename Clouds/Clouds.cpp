// Clouds.cpp: определяет точку входа для консольного приложения.
//

#define Tw \/\/

#include "stdafx.h"
#include <cmath>

#include "GlutApp.h"
#include "Triangle.h"
#include "Cube.h"
#include "Logger.h"
#include "UniformWrapper.h"
#include "Sphere.h"
#include "Camera.h"
#include "Axis.h"
#include "Plane.h"
#include "MouseCamera.h"
#include "Scene.h"
#include "Cloud.h"
#include "SkyBox.h"
#include "Mover.h"
#include "CloudsField.h"
#include "Particle.h"
#include "ParticleField.h"

Sphere sp;
Camera camera;
MouseCamera mouseCamera;
Axis   axis;
Plane  plane;
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
GLuint    CloudsCount    = 1;
GLfloat   CloudsCoverage = 0.5;
GLuint    ParticlesCount = 1000;

void keyboardFunc(unsigned char key, int x, int y);
void motionFunc(int x, int y);
void idleFunc();
void timerFunc(int value);
void reshapeFunc(int width, int height);
void closeFunc();

void TW_CALL initCloudsField(void *clientData);

glm::vec3 SimpleSinFunc(GLfloat time);
glm::vec3 SimpleLinkedFunc(GLfloat time);

int _tmain(int argc, _TCHAR* argv[])
{
	Logger::instance().setStream(std::cout);

	LOG_DEBUG("hello debug");
	LOG_ERROR("hello info");


	//ParticleField field(1.0f, 50.0f, 100.0f);
	//field.generate(1000);


	//return 0;

	GlutApp app(argc, argv, "Clouds App");

	GlutWindow wnd = app.getMainWindow();

//	TwInit(TW_OPENGL_CORE, NULL);
//	TwBar *myBar;
//	myBar = TwNewBar("Clouds Coverage");

	//TwAddVarRW(myBar, "CloudsCount",    TW_TYPE_UINT16, &CloudsCount, "");
	//TwAddVarRW(myBar, "CloudsCoverage", TW_TYPE_FLOAT,  &CloudsCoverage, "");
	//TwAddSeparator(myBar, "Left Bottom Far", "");
	//TwAddVarRW(myBar, "X1", TW_TYPE_FLOAT, &LeftBottomFar[0], "");
	//TwAddVarRW(myBar, "Y1", TW_TYPE_FLOAT, &LeftBottomFar[1], "");
	//TwAddVarRW(myBar, "Z1", TW_TYPE_FLOAT, &LeftBottomFar[2], "");
	//TwAddSeparator(myBar, "Right Top Near", "");
	//TwAddVarRW(myBar, "X2", TW_TYPE_FLOAT, &RightTopNear[0], "");
	//TwAddVarRW(myBar, "Y2", TW_TYPE_FLOAT, &RightTopNear[1], "");
	//TwAddVarRW(myBar, "Z2", TW_TYPE_FLOAT, &RightTopNear[2], "");
	//TwAddSeparator(myBar, "Particles", "");
	//TwAddVarRW(myBar, "Count",    TW_TYPE_UINT32, &ParticlesCount, "");
	//
	//TwAddButton(myBar, "Apply", &initCloudsField, 0, "");

	mouseCamera.setPos(glm::vec3(4, 0.0f, 0));
	mouseCamera.setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	mouseCamera.setHead(glm::vec3(0.0f, 1.0f, 0.0f));

	Scene scene;
	scene.setCamera(&mouseCamera);
	wnd.setScene(&scene);

	Shader cloudShader(wnd.getContext());
	cloudShader.loadFromFile("cloud", Shader::Vertex | Shader::Fragment);
	cloudShader.link();

	Shader billboardShader(wnd.getContext());
	billboardShader.loadFromFile("billboard", Shader::Vertex | Shader::Fragment);
	billboardShader.link();


	Shader* particleShader  = Particle::LoadShader(wnd.getContext());
	GLuint  particleVertex  = Particle::GetVertexBuffer(wnd.getContext());
	GLuint  particleTexture = Particle::GetTextureID(wnd.getContext());

	Particle particle(wnd.getContext(), particleShader, particleVertex, particleTexture);
	//scene.addObject(&particle);

	moverTest.setFunc(SimpleSinFunc);
	moverTest.addObject(&particle);

	//Cloud cloud(wnd.getContext());
	//cloud.setBillboardShader(&billboardShader);
	//cloud.setMainShader(&cloudShader);
	//cloud.setWidth(glm::vec3(2.0f, 1.0f, 1.0f));

	Shader skyboxShader(wnd.getContext());
	skyboxShader.loadFromFile("skybox", Shader::Vertex | Shader::Fragment);
	skyboxShader.link();

	cloudsField.setContext(wnd.getContext());
	cloudsField.setMainShader(&cloudShader);
	//cloudsField.addVarToBar(myBar);

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

	//TwGLUTModifiersFunc(glutGetModifiers);
 

	app.exec();
}

void keyboardFunc(unsigned char key, int x, int y)
{
	switch(key){
	case 'w':
		sp.move(glm::vec3(0.1, 0.0, 0.0));
		break;
	case 's':
		sp.move(glm::vec3(-0.1, 0.0, 0.0));
		break;

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

	case 'c':
		plane.move(glm::vec3(0.1, 0.0, 0.0));
		break;
	case 'v':
		plane.move(glm::vec3(-0.1, 0.0, 0.0));
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

	plane.setRotate(axis, angle);

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
		std::cout << deltaHor << std::endl;


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
	cloudsField.fill(CloudsCount, LeftBottomFar, RightTopNear, CloudsCoverage, ParticlesCount);
}