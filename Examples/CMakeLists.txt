
include_directories(${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})

add_executable(BlueWindow src/BlueWindow.cpp)

target_link_libraries(BlueWindow PRIVATE SDL2::SDL2 SDL2::SDL2main GLEW::GLEW ${OPENGL_LIBRARIES})

add_executable(BlueWindowTriangle src/BlueWindowTriangle.cpp)

target_link_libraries(BlueWindowTriangle PRIVATE SDL2::SDL2 SDL2::SDL2main GLEW::GLEW ${OPENGL_LIBRARIES})

add_executable(RedTriangle src/RedTriangle.cpp)

target_link_libraries(RedTriangle PRIVATE SDL2::SDL2 SDL2::SDL2main GLEW::GLEW ${OPENGL_LIBRARIES})

add_executable(MultiColoredTriangle src/MultiColoredTriangle.cpp src/GlslCompiler.cpp)

target_link_libraries(MultiColoredTriangle PRIVATE SDL2::SDL2 SDL2::SDL2main GLEW::GLEW ${OPENGL_LIBRARIES})

configure_file(glsl/multicoloredtriangle.frag ${CMAKE_BINARY_DIR}/Examples/multicoloredtriangle.frag COPYONLY)
configure_file(glsl/multicoloredtriangle.vert ${CMAKE_BINARY_DIR}/Examples/multicoloredtriangle.vert COPYONLY)

add_executable(MovingTriangle src/MovingTriangle.cpp src/GlslCompiler.cpp)

target_link_libraries(MovingTriangle PRIVATE SDL2::SDL2 SDL2::SDL2main GLEW::GLEW ${OPENGL_LIBRARIES})

configure_file(glsl/movingshape.frag ${CMAKE_BINARY_DIR}/Examples/movingshape.frag COPYONLY)
configure_file(glsl/movingshape.vert ${CMAKE_BINARY_DIR}/Examples/movingshape.vert COPYONLY)

add_executable(Textures src/Textures.cpp src/GlslCompiler.cpp)

target_link_libraries(Textures PRIVATE SDL2::SDL2 SDL2::SDL2main GLEW::GLEW ${OPENGL_LIBRARIES})

configure_file(glsl/textures.frag ${CMAKE_BINARY_DIR}/Examples/textures.frag COPYONLY)
configure_file(glsl/textures.vert ${CMAKE_BINARY_DIR}/Examples/textures.vert COPYONLY)
configure_file(textures/Floor.png ${CMAKE_BINARY_DIR}/Examples/Floor.png COPYONLY)