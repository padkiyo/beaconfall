#include "window.h"

Window::Window(i32 width, i32 height, const std::string& title)
	: m_width(width), m_height(height), m_title(title) {

	panic(SDL_Init(SDL_INIT_EVERYTHING) == 0, "Failed to initialize SDL");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// TODO(slok): Make configurable window attributes
	m_sdl_window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);
	panic(m_sdl_window, "Failed to create SDL Window");

	// Loading the opengl functions
	m_gl_context = SDL_GL_CreateContext(m_sdl_window);
	m_gl_version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
	panic(m_gl_version != 0, "Failed to load Opengl functions");
}

Window::~Window() {
	SDL_GL_DeleteContext(m_gl_context);
	SDL_DestroyWindow(m_sdl_window);
	SDL_Quit();
}

void Window::swap() {
	SDL_GL_SwapWindow(m_sdl_window);
}
