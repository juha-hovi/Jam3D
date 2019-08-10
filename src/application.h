#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>

class Application
{
public:
	Application();
	~Application();

	void Run();
	int InitOpenGL(GLFWwindow* window);

	void GLAPIENTRY OpenGLDebugCallback(GLenum source,
                                    	GLenum type,
                                    	GLuint id,
                                    	GLenum severity,
                                    	GLsizei length,
                                    	const GLchar* message,
                                    	const void* userParam)

	GLFWwindow* m_Window;
	unsigned int m_Width;
	unsigned int m_Height;
	std::string m_Title;
};