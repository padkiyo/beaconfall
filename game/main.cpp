#include "core.h"
#include "common.h"

int main(int argc, char* argv[]) {
	Window window = window_create(640, 480, "Game6 v0.0").unwrap();
	bool running = true;
	SDL_Event event;

	log::info("Opengl Version:");
	log::info(window_gl_version(window).c_str());

	// Main loop
	while(running){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
			}
		}

		glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
		window_swap(window);
	}
	window_destroy(&window);

	return 0;
}
