cmake_minimum_required(VERSION 3.0)
project(opengl_project_app)

add_subdirectory(renderer)
include_directories(renderer)

add_subdirectory(shapes)
include_directories(shapes)

add_subdirectory(tests)
include_directories(tests)

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})


set(app_source 
	entrypoint.cpp
	application.cpp
	application.h)
add_executable(app ${app_source})

target_link_libraries(app tests imgui glfw ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})