#pragma once

#include <array>
#include <vector>
#include "Scene.h"

struct GlutWindowData;
class GlutApp;

class GlutWindow
{
public:
	GlutWindow(const GlutWindow& window);
	~GlutWindow(void);

	GLContext getContext() const;

	void setScene(Scene* scene);

private:
	GlutWindow(int windowId);

	friend GlutApp;

	GlutWindowData* data;
	int*            counter;

	static const int MaxWindowsCount = 256;
	static std::array<Scene*, MaxWindowsCount> Windows;

	static void DisplayFunc();
	static void ReshapeFunc(int width, int height);
};

