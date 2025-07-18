#pragma once

#include "core.h"

class Player {
public:
	Player();
	~Player();

	// Setters
	void set_pos(const glm::vec2& pos);
	void set_speed(f32 speed);

	void handle_event(const SDL_Event& event);
	void update(const std::vector<Rect>& boxes, f64 dt);
	Quad render();

private:
	Rect m_rect;

	// Movement
	glm::vec2 m_move;
	f32 m_speed;
	b32 m_left, m_right, m_up, m_down;

	// Graphics
	Texture* m_texture;
};
