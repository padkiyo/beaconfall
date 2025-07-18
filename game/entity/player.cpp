#include "player.h"

Player::Player()
	: Entity(ENT_PLAYER) {
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = 32;
	m_rect.h = 32;

	m_attack = false;
	m_move = { 0, 0 };
	m_speed = 200.0f;
	m_left = m_right = m_up = m_down = false;
}

Player::~Player() {
}

void Player::set_pos(const glm::vec2& pos) {
	m_rect.x = pos.x;
	m_rect.y = pos.y;
}

void Player::set_speed(f32 speed) {
	m_speed = speed;
}

void Player::handle_event(const SDL_Event& event) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
			case SDLK_w:
				m_up = true;
				break;
			case SDLK_s:
				m_down = true;
				break;
			case SDLK_a:
				m_left = true;
				break;
			case SDLK_d:
				m_right = true;
				break;
		}
	}
	else if (event.type == SDL_KEYUP) {
		switch (event.key.keysym.sym) {
			case SDLK_w:
				m_up = false;
				break;
			case SDLK_s:
				m_down = false;
				break;
			case SDLK_a:
				m_left = false;
				break;
			case SDLK_d:
				m_right = false;
				break;
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			m_attack = true;
		}
	}
}

void Player::update(const std::vector<Entity*>& entities, f64 dt) {
	// Movement
	if (m_left)
		m_move.x -= m_speed;
	if (m_right)
		m_move.x += m_speed;
	if (m_up)
		m_move.y += m_speed;
	if (m_down)
		m_move.y -= m_speed;

	// Attack
	if (m_attack) {
	}

	// Collision resolution
	m_rect.x += m_move.x * dt;
	for (auto ent: entities) {
		m_rect.resolve_x(ent->rect(), m_move);
	}

	m_rect.y += m_move.y * dt;
	for (auto ent: entities) {
		m_rect.resolve_y(ent->rect(), m_move);
	}

	// Reset the movement
	m_move = {0,0};
}

Quad Player::render(const SpriteManager& sprt_mgr) {
	return Quad {
		.pos = {m_rect.x, m_rect.y, 0},
		.size = {m_rect.w, m_rect.h},
		.rot = glm::mat4(1),
		.texture = &sprt_mgr.get_spritesheet_texture(PLAYER),
		.uv = {0, 0, 1, 1},
		.color = {1,1,1,1},
	};
}
