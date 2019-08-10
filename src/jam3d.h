#pragma once

#include "GLFW/glfw3.h"

#include <string>

class Jam3D
{
public:
	Jam3D();
	~Jam3D();

	void Run();
	int InitOpenGL();

	GLFWwindow* m_Window;
	unsigned int m_Width;
	unsigned int m_Height;
	std::string m_Title;
};