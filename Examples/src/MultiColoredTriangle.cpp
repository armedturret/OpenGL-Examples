#include "GlslCompiler.h"

#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>

ShaderProgram  myProgram;
GLuint vboId = 0;


struct Position {
	GLfloat x;
	GLfloat y;
};

struct Color {
	GLubyte r;
	GLubyte g;
	GLubyte b;
};

struct Vertex {
	Position position;
	Color color;
};


void genBuffers() {
	//lets create an array of verticies
	Vertex triangleVertices[] = {
		{{-0.5f, 0.0f}, {GLubyte(255), 0, 0}},
		{{0.0f, 1.0f}, {0, GLubyte(255), 0}},
		{{0.5f, 0.0f}, {0, 0, GLubyte(255)}}
	};

	glGenBuffers(1, &vboId);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), &triangleVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw() {
	GLuint program = myProgram.getProgram();
	glUseProgram(program);

	//use our triangle data
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	
	//get our attribute locations
	GLuint attribPos = glGetAttribLocation(program, "in_Position");
	GLuint attribColor = glGetAttribLocation(program, "in_Color");

	//enable our OpenGL attributes
	glEnableVertexAttribArray(attribPos);
	glEnableVertexAttribArray(attribColor);

	//tell the program 
	glVertexAttribPointer(attribPos, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(attribColor, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	//draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//disable the attributes
	glEnableVertexAttribArray(attribPos);
	glEnableVertexAttribArray(attribColor);

	//clear binds
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);
}

int main(int argc, char** argv) {
	

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cout << "Error: " << SDL_GetError() << std::endl;
	}
	else {
		std::cout << "SDL Initialized" << std::endl;
	}
	
	SDL_Window *window = SDL_CreateWindow("Triangles are a Thing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

	SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	else {
		std::cout << "glew Initialized" << std::endl;
	}

	std::cout << "OpenGL version: " << (char*)(glGetString(GL_VERSION)) << std::endl;
	
	myProgram.compileShadersFromFile("multicoloredtriangle.vert", "multicoloredtriangle.frag");

	genBuffers();

	bool shouldQuit = false;
	while (!shouldQuit) {
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw();

		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) {
			if (evnt.type == SDL_QUIT)
				shouldQuit = true;
		}

		SDL_GL_SwapWindow(window);
	}

	//cleanup
	glDeleteBuffers(1, &vboId);

	myProgram.destroy();

	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}