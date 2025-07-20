#include "entity.h"


Entity::Entity(EntityType type): type(type)
{}

Entity::~Entity()
{}

void Entity::set_pos(const glm::vec2& pos) {
	m_rect.x = pos.x;
	m_rect.y = pos.y;
}

void Entity::set_size(const glm::vec2& size) {
	m_rect.w = size.x;
	m_rect.h = size.y;
}

void Entity::take_damage(f32 dmg) {
	m_health -= dmg;

	if (m_health <= 0.0f)
		m_health = 0.0f;

	// Give a stun
	if (m_stun == 0.0f)
		m_stun = m_stun_timeout;
	
	on_damage();
}

b32 Entity::is_stunned(f64 dt) {
	m_stun -= m_stun_decay * dt;
	if (m_stun <= 0.0f) m_stun = 0.0f;
	return m_stun > 0.0f;
}

void Entity::on_damage() {
}

void Entity::render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads, std::vector<Light>& lights) {
	panic(false, "Donot call this function directly");
}
