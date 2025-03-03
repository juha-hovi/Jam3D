cmake_minimum_required(VERSION 3.0)
project(Jam3D)

include_directories(math)

add_subdirectory(misc)
include_directories(misc)

add_subdirectory(renderer)
include_directories(renderer)

add_subdirectory(objects)
include_directories(objects)

add_subdirectory(views)
include_directories(views)

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

set(jam3d_source 
	entrypoint.cpp
	application.cpp
	application.h)
add_executable(Jam3D ${jam3d_source})

target_link_libraries(Jam3D renderer views imgui glfw ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})