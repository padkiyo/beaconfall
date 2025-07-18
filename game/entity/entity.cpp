#include "entity.h"


Entity::Entity(EntityID id): id(id)
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

Quad Entity::render(const SpriteManager& sprt_mgr) {
	panic(false, "Donot call this function directly");
	return Quad {};
}
