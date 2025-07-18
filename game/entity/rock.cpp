#include "rock.h"

Rock::Rock()
	: Entity(ENT_ROCK) {
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = 32;
	m_rect.h = 32;
}

Rock::~Rock() {
}

void Rock::set_pos(const glm::vec2& pos) {
	m_rect.x = pos.x;
	m_rect.y = pos.y;
}

Quad Rock::render(const SpriteManager& sprt_mgr) {
	return Quad {
		.pos = {m_rect.x, m_rect.y, 0},
		.size = {m_rect.w, m_rect.h},
		.rot = glm::mat4(1),
		.texture = &sprt_mgr.get_spritesheet_texture(ROCK),
		.uv = {0, 0, 1, 1},
		.color = {1,1,1,1},
	};
}

