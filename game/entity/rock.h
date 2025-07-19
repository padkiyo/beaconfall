#pragma once

#include "core.h"
#include "entity.h"
#include "systems/sprite_system/sprite_system.h"

class Rock : public Entity {
public:
	Rock(std::vector<Entity*>& entities);
	~Rock();

	// Setters
	void set_pos(const glm::vec2& pos);

	void on_damage();
	void render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads);

private:
	std::vector<Entity*>& m_entities;
};
