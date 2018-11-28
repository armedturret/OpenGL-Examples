#include <iostream>
#include <SDL2/SDL.h>
#include <gl/glew.h>

int main(int argc, char** argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cout << "This ain't it chief" << std::endl;
	}
	else {
		std::cout << "SDL Initialized" << std::endl;
	}

	SDL_Window *window = SDL_CreateWindow("I am blue!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	
	SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		std::cout << "This also ain't it chief" << std::endl;
	}
	else {
		std::cout << "glew Initialized" << std::endl;
	}
	
	bool shouldQuit = false;
	while (!shouldQuit) {
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) {
			if (evnt.type == SDL_QUIT)
				shouldQuit = true;
		}

		SDL_GL_SwapWindow(window);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}