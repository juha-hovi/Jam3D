cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
project(opengl_project)

include_directories(vendor)

add_subdirectory(src)
add_subdirectory(vendor/stb_image)
add_subdirectory(vendor/imgui)