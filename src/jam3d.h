#pragma once

#include "glwindow.h"
#include "testbox.h"

#include "GLFW/glfw3.h"

#include <string>
#include <memory>

namespace Jam3D {

class Jam3D
{
public:
	Jam3D();
	~Jam3D();

	void Run();

	std::unique_ptr<GLWindow> m_GLWindow;
	std::unique_ptr<TestBox> m_Test;
};

}