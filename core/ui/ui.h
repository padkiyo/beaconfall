#pragma once

#include "common.h"
#include "shader/shader.h"
#include "camera/camera.h"
#include "renderer/renderer.h"
#include "rect/rect.h"

struct Style {
	glm::vec4 bg_color = { 0.3, 0.3, 0.3, 1 };
	glm::vec4 hover_color = { 0.6, 0.6, 0.6, 1 };
	glm::vec4 fg_color = { 1, 1, 1, 1 };
	Font* font = nullptr;
};

class UI {
public:
	UI(Renderer* renderer, const glm::vec2& resolution);
	~UI();

	void begin();
	void end();
	void handle_event(const SDL_Event& event);

	void text(const std::string& label, const glm::vec2& pos, const Style& style);
	b32 button(const std::string& label, const Rect& rect, const Style& style);
	void progress_bar(const Rect& rect, f32 value, f32 max, const Style& style);

private:
	Renderer* m_renderer;
	glm::vec2 m_res;
	Camera* m_camera;
	Shader* m_shader;

	// Event states
	b32 m_mouse_clicked = false;
};
