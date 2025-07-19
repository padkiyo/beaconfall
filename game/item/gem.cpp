#include "gem.h"

Gem::Gem(std::vector<Entity*>& entities)
	: Entity(ENT_GEM), m_entities(entities) {
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = 32;
	m_rect.h = 32;
	m_rect.set_collidable(false);
}

Gem::~Gem() {
}

void Gem::render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads) {
	quads.push_back(Quad {
		{ m_rect.x, m_rect.y, 0 },
		{ m_rect.w, m_rect.h },
		glm::mat4(1),
		&sprt_mgr.get_spritesheet_texture(GEM),
		{ 0, 0, 1, 1 },
		{ 1, 1, 1, 1 },
	});
}
