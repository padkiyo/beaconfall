#include <glad/gl.h>
#include <SDL2/SDL.h>
#include "../core/base/log.h"

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		log::panic("Unable to initialize SDL");
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	SDL_Window* window = SDL_CreateWindow(
			"Test window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			640, 480,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);

	if(!window){
		SDL_Quit();
		log::panic("Unable to create a SDL window");
	}

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(1);

	int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);

	if(version == 0){
		SDL_DestroyWindow(window);
		SDL_Quit();
		log::panic("Unable to create GLAD context");
	}

	printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

	bool running = true;
	SDL_Event event;

	// Main loop
	while(running){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
			}
		}

		glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
	}
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
