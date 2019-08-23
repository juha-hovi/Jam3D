#pragma once

#include "glwindow.h"
#include "testview.h"

#include "GLFW/glfw3.h"

#include <string>
#include <memory>

namespace Jam3D {

class Application
{
public:
	Application();
	~Application();

	void Run();

	std::shared_ptr<GLWindow> m_GLWindow;
	std::unique_ptr<TestView> m_TestView;

};

}