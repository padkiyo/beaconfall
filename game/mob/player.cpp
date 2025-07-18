#include "player.h"

Player::Player() {
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = 32;
	m_rect.h = 32;

	m_move = { 0, 0 };
	m_speed = 200.0f;
	m_left = m_right = m_up = m_down = false;

	m_texture = new Texture("./assets/player.png", TextureFilter {
		.min_filter = GL_NEAREST,
		.mag_filter = GL_NEAREST,
		.wrap_s = GL_CLAMP_TO_EDGE,
		.wrap_t = GL_CLAMP_TO_EDGE,
		.flip = true,
	});
}

Player::~Player() {
	delete m_texture;
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
}

void Player::update(const std::vector<Rect>& boxes, f64 dt) {
	if (m_left)
		m_move.x -= m_speed;
	if (m_right)
		m_move.x += m_speed;
	if (m_up)
		m_move.y += m_speed;
	if (m_down)
		m_move.y -= m_speed;

	m_rect.resolve(boxes, m_move, dt);
	m_move = {0,0};
}

Quad Player::render() {
	m_texture->bind();

	return Quad {
		.pos = {m_rect.x, m_rect.y, 0},
		.size = {m_rect.w, m_rect.h},
		.rot = glm::mat4(1),
		.texture = m_texture,
		.uv = {0, 0, 1, 1},
		.color = {1,1,1,1},
	};
}
