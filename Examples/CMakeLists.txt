

add_executable(BlueWindow src/BlueWindow.cpp)

target_include_directories(BlueWindow 
	PRIVATE ${SDL_INCLUDE_DIR} 
	PRIVATE ${GLEW_INCLUDE_DIR})

target_link_libraries(BlueWindow ${OPENGL_LIBRARIES} ${SDL_LIB} ${GLEW_LIB})

add_executable(BlueWindowTriangle src/BlueWindowTriangle.cpp)

target_include_directories(BlueWindowTriangle 
	PRIVATE ${SDL_INCLUDE_DIR} 
	PRIVATE ${GLEW_INCLUDE_DIR})

target_link_libraries(BlueWindowTriangle ${OPENGL_LIBRARIES} ${SDL_LIB} ${GLEW_LIB})
