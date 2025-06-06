#include "window.h"

Result<Window*, const char*> window_create(i32 width, i32 height, const char* title) {

	// ! For returning from function
	Window* res_window = new Window;

	// Initialization stuff incomming
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		return "Unable to initialize SDL";
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Window* window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);

	// Checking if window is created
	if(!window){
		SDL_Quit();
		return "Unable to create a SDL window";
	}

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);

	// Checking if glad is initliazed or not
	if(version == 0){
		SDL_DestroyWindow(window);
		SDL_Quit();
		return "Unable to create GLAD context";
	}

	res_window->width = width;
	res_window->height = height;
	res_window->title = title;
	res_window->version = version;
	res_window->sdl_window = window;
	res_window->gl_context = gl_context;

	return res_window;
}

// Swaps window after rendering
void window_swap(Window* window) {
	SDL_GL_SwapWindow(window->sdl_window);
}

// Function to print gl version
std::string  window_gl_version(Window* window){

	// Weird code ahead! Pls Ignore :D
	std::string version;
	version += std::to_string(GLAD_VERSION_MAJOR(window->version));
	version += ".";
	version += std::to_string(GLAD_VERSION_MINOR(window->version));


	return version;
}

// Clean up function
void window_destroy(Window* window){
	SDL_GL_DeleteContext(window->gl_context);
	SDL_DestroyWindow(window->sdl_window);
	SDL_Quit();
	delete window;
}
