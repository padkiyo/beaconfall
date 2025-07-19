#include "rock.h"

Rock::Rock(std::vector<Entity*>& entities)
	: Entity(ENT_ROCK), m_entities(entities) {
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = 32;
	m_rect.h = 32;

	m_health = 100.0f;
}

Rock::~Rock() {
}

void Rock::set_pos(const glm::vec2& pos) {
	m_rect.x = pos.x;
	m_rect.y = pos.y;
}

void Rock::on_damage() {
	if (m_health <= 0.0f) {
		// NOTE(slok): This looks dangerous if undefined behaviour occured change this
		m_entities.erase(
			std::remove(m_entities.begin(), m_entities.end(), this),
			m_entities.end()
		);

		// THIS IS DANGEROUS
		delete this;
	}
}

void Rock::render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads) {
	quads.push_back(Quad {
		.pos = {m_rect.x, m_rect.y, 0},
		.size = {m_rect.w, m_rect.h},
		.rot = glm::mat4(1),
		.texture = &sprt_mgr.get_spritesheet_texture(ROCK),
		.uv = {0, 0, 1, 1},
		.color = {1,1,1,1},
	});
}

