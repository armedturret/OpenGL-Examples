
include_directories(${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

add_executable(BlueWindow src/BlueWindow.cpp)

target_link_libraries(BlueWindow PRIVATE SDL2::SDL2 SDL2::SDL2main GLEW::GLEW ${OPENGL_LIBRARIES})

add_executable(BlueWindowTriangle src/BlueWindowTriangle.cpp)

target_link_libraries(BlueWindowTriangle PRIVATE SDL2::SDL2 SDL2::SDL2main GLEW::GLEW ${OPENGL_LIBRARIES})

add_executable(RedTriangle src/RedTriangle.cpp)

target_link_libraries(RedTriangle PRIVATE SDL2::SDL2 SDL2::SDL2main GLEW::GLEW ${OPENGL_LIBRARIES})
