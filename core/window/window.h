#include "common.h"

struct Window {
		i32 width;
		i32 height;
		const char*  title;
		SDL_Window* sdl_window;
		SDL_GLContext gl_context;
		i32 version;
};

Result<Window, const char*> window_create(i32 width, i32 height, const char* title);

void window_swap(Window window);
std::string window_gl_version(Window window);
void window_destroy(Window* window);
