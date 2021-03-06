#pragma once

class GLContext;

class GLContextGetter
{
public:
	GLContextGetter(const GLContext& context);
	~GLContextGetter(void);
private:
	int oldWindowId;
};

class GLContext
{
public:
	friend GLContextGetter;

	GLContext(int _windowId):windowId(_windowId){}
	GLContext():windowId(-1){}
private:
	int windowId;
};

