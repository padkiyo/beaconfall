#include "common.h"

class Window {
public:
	Window(i32 width, i32 height, const std::string& title);
	~Window();
	void swap();

public:
	inline std::string get_gl_version() const {
		return std::format("{}.{}",
			std::to_string(GLAD_VERSION_MAJOR(m_gl_version)),
			std::to_string(GLAD_VERSION_MINOR(m_gl_version)));
	}

	inline SDL_Window* get_sdl_window()   const { return m_sdl_window; }
	inline SDL_GLContext get_gl_context() const { return m_gl_context; }

private:
	i32 m_width, m_height;
	std::string m_title;

	SDL_Window* m_sdl_window;
	SDL_GLContext m_gl_context;
	i32 m_gl_version;
};
