project(openglexamples)

cmake_minimum_required(VERSION 3.11.0)

find_package(SDL2 REQUIRED)
find_package(glew REQUIRED)

find_package(OpenGL REQUIRED)

add_subdirectory(Examples)