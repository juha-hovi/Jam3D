#pragma once

#include "glwindow.h"
#include "testview.h"
#include "objectcreationview.h"

#include "GLFW/glfw3.h"

#include <string>
#include <memory>
#include <vector>

namespace Jam3D {

class Application
{
public:
	Application();
	~Application();

	void Run();

	std::shared_ptr<GLWindow> m_GLWindow;

	std::shared_ptr<TestView> m_TestView;
	std::shared_ptr<ObjectCreationView> m_ObjectCreationView;

	std::shared_ptr<View> m_CurrentView;
};

}