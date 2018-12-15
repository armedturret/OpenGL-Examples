#include "GlslCompiler.h"

#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float PI = 3.14159265359f;

const float SPEED = 0.05f;
const float ROTATION_SPEED = 3.0f;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

ShaderProgram  myProgram;
GLuint vboId = 0;
GLfloat time = 0.0f;
glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);
glm::mat4 projectionMatrix = glm::mat4(1.0f);

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

const Color RED = { GLubyte(255), 0, 0 };

void genBuffers() {
	//lets create an array of verticies
	Vertex triangleVertices[] = {
		{{-0.5f, 0.0f}, RED},
		{{0.0f, 1.0f}, RED},
		{{0.5f, 0.0f}, RED}
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
	GLint attribPos = glGetAttribLocation(program, "in_Position");
	if (attribPos == -1) {
		std::cout << "in_Position not found!" << std::endl;
	}

	GLint attribColor = glGetAttribLocation(program, "in_Color");
	if (attribColor == -1) {
		std::cout << "in_Color not found!" << std::endl;
	}

	GLint uniformTime = glGetUniformLocation(program, "time");
	if (uniformTime != -1) {
		glUniform1fv(uniformTime, 1, &time);
	}

	GLint uniformMVP = glGetUniformLocation(program, "mvp");
	if (uniformMVP == -1) {
		std::cout << "mvp not found!" << std::endl;
	}

	glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp));
	//enable our OpenGL attributes
	glEnableVertexAttribArray(attribPos);
	glEnableVertexAttribArray(attribColor);

	//tell the program 
	glVertexAttribPointer(attribPos, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(attribColor, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	//draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//disable the attributes
	glDisableVertexAttribArray(attribPos);
	glDisableVertexAttribArray(attribColor);

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

	SDL_Window *window = SDL_CreateWindow("It can move?", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

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

	myProgram.compileShadersFromFile("movingshape.vert", "movingshape.frag");

	genBuffers();

	GLfloat aspectRatio = GLfloat(SCREEN_WIDTH) / GLfloat(SCREEN_HEIGHT);

	//calculate our matricies
	viewMatrix = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, 0.1f, 100.0f);

	bool shouldQuit = false;
	while (!shouldQuit) {
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw();

		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) {
			if (evnt.type == SDL_QUIT) {
				shouldQuit = true;
			}
			else if (evnt.type == SDL_KEYDOWN) {
				//move the triangle based off keyboard input
				if (evnt.key.keysym.sym == SDLK_w || evnt.key.keysym.sym == SDLK_UP) {
					//transform up
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, SPEED, 0.0f));
				}
				if(evnt.key.keysym.sym == SDLK_s || evnt.key.keysym.sym == SDLK_DOWN){
					//transform down
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -SPEED, 0.0f));
				}
				if (evnt.key.keysym.sym == SDLK_a || evnt.key.keysym.sym == SDLK_LEFT) {
					//rotate left
					modelMatrix = glm::rotate(modelMatrix, ((ROTATION_SPEED * PI) / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				}
				if (evnt.key.keysym.sym == SDLK_d || evnt.key.keysym.sym == SDLK_RIGHT) {
					//rotate right
					modelMatrix = glm::rotate(modelMatrix, -((ROTATION_SPEED * PI) / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				}
			}

		}

		time += 0.001f;

		SDL_GL_SwapWindow(window);
	}

	//cleanup
	glDeleteBuffers(1, &vboId);

	myProgram.destroy();

	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}