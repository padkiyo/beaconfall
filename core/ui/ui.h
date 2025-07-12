#pragma once

#include "common.h"
#include "shader/shader.h"
#include "camera/camera.h"
#include "renderer/renderer.h"

struct Style {
	glm::vec4 fg_color = { 1, 1, 1, 1 };
	Font* font = nullptr;
};

class UI {
public:
	UI(Renderer* renderer, const glm::vec2& resolution);
	~UI();

	void begin();
	void end();

	b32 button();
	void text(const std::string& msg, const glm::vec2& pos, const Style& style);

private:
	Renderer* m_renderer;
	glm::vec2 m_res;
	Camera* m_camera;
	Shader* m_shader;
};
