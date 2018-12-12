#include "GlslCompiler.h"

#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const float PI = 3.14159265359f;

const float SPEED = 0.05f;
const float ROTATION_SPEED = 3.0f;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


ShaderProgram  myProgram;
GLuint vboId = 0;
GLuint textureId = 0;
GLfloat time = 0.0f;
glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);
glm::mat4 projectionMatrix = glm::mat4(1.0f);

struct Position {
	GLfloat x;
	GLfloat y;
};

struct UV {
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
	UV uv;
};

const Color RED = { GLubyte(255), 0, 0 };
const Color WHITE{ GLubyte(255), GLubyte(255), GLubyte(255) };

struct UvBox {
	UV topLeft;
	UV topRight;
	UV bottomRight;
	UV bottomLeft;
};

UvBox getTileUV(int tile, int srcWidth, int srcHeight) {
	GLfloat internalTilex = GLfloat((tile) % srcWidth) + 1.0f;
	GLfloat internalTiley = GLfloat(tile/srcWidth);

	UvBox tmp;
	
	//get width
	GLfloat tileWidth = 1.0f / GLfloat(srcWidth);
	GLfloat tileHeight = 1.0f / GLfloat(srcHeight);

	//calculate corners
	tmp.bottomLeft.x = (internalTilex - 1.0f) * tileWidth;
	tmp.bottomLeft.y = (internalTiley - 1.0f) * tileHeight;

	tmp.bottomRight.x = internalTilex * tileWidth;
	tmp.bottomRight.y = (internalTiley - 1.0f) * tileHeight;

	tmp.topLeft.x = (internalTilex - 1.0f) * tileWidth;
	tmp.topLeft.y = internalTiley * tileHeight;

	tmp.topRight.x = internalTilex * tileWidth;
	tmp.topRight.y = internalTiley * tileHeight;

	return tmp;
}

void genBuffers() {
	UvBox animationUV = getTileUV(0, 3, 2);

	//create vertex data
	Vertex triangleVertices[] = {
		{{-0.5f, -0.5f}, WHITE, animationUV.bottomLeft },
		{{-0.5f, 0.5f}, WHITE, animationUV.topLeft},
		{{0.5f, 0.5f}, WHITE, animationUV.topRight},
		{{0.5f, 0.5f}, WHITE, animationUV.topRight},
		{{-0.5f, -0.5f}, WHITE, animationUV.bottomLeft},
		{{0.5f, -0.5f}, WHITE, animationUV.bottomRight}
	};

	glGenBuffers(1, &vboId);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), &triangleVertices, GL_DYNAMIC_DRAW);

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

	GLint attribUV = glGetAttribLocation(program, "in_UV");
	if (attribUV == -1) {
		std::cout << "in_UV not found!" << std::endl;
	}

	GLint uniformTime = glGetUniformLocation(program, "time");
	if (uniformTime != -1) {
		glUniform1fv(uniformTime, 1, &time);
	}

	GLint uniformMVP = glGetUniformLocation(program, "mvp");
	if (uniformMVP == -1) {
		std::cout << "mvp not found!" << std::endl;
	}

	GLint uniformSampler = glGetUniformLocation(program, "mySampler");
	if (uniformSampler == -1) {
		std::cout << "mySampler not found!" << std::endl;
	}

	//get animation UV
	UvBox animationUV = getTileUV((int)floor(time)%3 + 3, 3, 2);

	//create vertex data
	Vertex vertices[] = {
		{{-0.5f, -0.5f}, WHITE, animationUV.bottomLeft },
		{{-0.5f, 0.5f}, WHITE, animationUV.topLeft},
		{{0.5f, 0.5f}, WHITE, animationUV.topRight},
		{{0.5f, 0.5f}, WHITE, animationUV.topRight},
		{{-0.5f, -0.5f}, WHITE, animationUV.bottomLeft},
		{{0.5f, -0.5f}, WHITE, animationUV.bottomRight}
	};

	//update vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_DYNAMIC_DRAW);

	glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp));

	//set our active texture
	glActiveTexture(GL_TEXTURE0);

	//bind our texture
	glBindTexture(GL_TEXTURE_2D, textureId);
	//enable our OpenGL attributes
	glEnableVertexAttribArray(attribPos);
	glEnableVertexAttribArray(attribColor);
	glEnableVertexAttribArray(attribUV);

	//tell the program 
	glVertexAttribPointer(attribPos, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(attribColor, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(attribUV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	//draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//disable the attributes
	glEnableVertexAttribArray(attribPos);
	glEnableVertexAttribArray(attribColor);
	glDisableVertexAttribArray(attribUV);

	//clear binds
	glBindTexture(GL_TEXTURE_2D, 0);
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

	SDL_Window *window = SDL_CreateWindow("Animation Time", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

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

	//enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//flip images to fix UV glitch
	stbi_set_flip_vertically_on_load(true);

	//generate the texture
	glGenTextures(1, &textureId);

	//load texture to generate settings
	glBindTexture(GL_TEXTURE_2D, textureId);

	//set up texture to repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//sets texture to have pixelated values when resized
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//load the texture we are going to use from the file
	int width, height, nrChannels;
	unsigned char *data = stbi_load("Mage.png", &width, &height, &nrChannels, 4);

	//load texture into OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	//free data used by stbi
	stbi_image_free(data);

	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	myProgram.compileShadersFromFile("textures.vert", "textures.frag");

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