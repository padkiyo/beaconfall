#include "gem.h"

Gem::Gem(std::vector<Entity*>& entities, GemType gem_type)
	: Entity(ENT_GEM), m_entities(entities), gem_type(gem_type) {
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = 32;
	m_rect.h = 32;
	m_rect.set_collidable(false);
}

Gem::~Gem() {
}

f32 Gem::get_value() {
	switch (gem_type) {
		case GEM_POWER: return 10.0f;
		case GEM_EXP: return 10.0f;
	}
	return 0.0f;
}

void Gem::render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads, std::vector<Light>& lights) {
	glm::vec4 color = { 1, 1, 1, 1 };

	switch (gem_type) {
		case GEM_POWER:
			color = { 0, 1, 1, 1 };
			break;
		case GEM_EXP:
			color = { 0, 1, 0.2, 1 };
	}

	quads.push_back(Quad {
		{ m_rect.x, m_rect.y, 0 },
		{ m_rect.w, m_rect.h },
		glm::mat4(1),
		&sprt_mgr.get_spritesheet_texture(GEM),
		{ 0, 0, 1, 1 },
		color,
	});
}
