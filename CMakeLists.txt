project(openglexamples)

cmake_minimum_required(VERSION 3.11.0)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

ExternalProject_Add(SDL
  URL https://www.libsdl.org/release/SDL2-2.0.9.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/deps/SDL
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(SDL install_dir)
set(SDL_INCLUDE_DIR ${install_dir}/include)
if(WIN32)
	set(SDL_LIB ${install_dir}/lib/SDL2d.lib ${install_dir}/lib/SDL2maind.lib)
else()
	set(SDL_LIB ${install_dir}/lib/SDL2d.a ${install_dir}/lib/SDL2maind.a)
endif()

ExternalProject_Add(glew
  URL https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.tgz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/deps/glew
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  SOURCE_SUBDIR build/cmake
)

ExternalProject_Get_Property(glew install_dir)
set(GLEW_INCLUDE_DIR ${install_dir}/include)
if(WIN32)
	set(GLEW_LIB ${install_dir}/lib/glew32.lib)
else()
	set(GLEW_LIB ${install_dir}/lib/libglew32.a)
endif()

find_package(OpenGL REQUIRED)

add_subdirectory(Examples)