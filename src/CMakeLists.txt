cmake_minimum_required(VERSION 2.8)
project(opengl_project_app)

add_subdirectory(renderer)

set(app_source application.cpp)
add_executable(app ${app_source})

target_link_libraries(app renderer)