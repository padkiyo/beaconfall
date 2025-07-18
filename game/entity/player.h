#pragma once

#include "core.h"
#include "systems/sprite_system/sprite_system.h"
#include "entity.h"

class Player : public Entity {
public:
	Player();
	~Player();

	// Setters
	void set_pos(const glm::vec2& pos);
	void set_speed(f32 speed);

	void handle_event(const SDL_Event& event);
	void update(const std::vector<Entity*>& entities, f64 dt);
	Quad render(const SpriteManager& sprt_mgr);

private:
	Rect m_rect;

	// Movement
	glm::vec2 m_move;
	f32 m_speed;
	b32 m_left, m_right, m_up, m_down;

	// Attack
	b32 m_attack;
	Rect m_atk_hitbox;
};
