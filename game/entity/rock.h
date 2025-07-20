#pragma once

#include "core.h"
#include "entity.h"
#include "systems/sprite_system/sprite_system.h"

class Rock : public Entity {
public:
	Rock(std::vector<Entity*>& entities);
	~Rock();

	void on_damage();
	void render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads, std::vector<Light>& lights);

private:
	std::vector<Entity*>& m_entities;
	glm::vec4 m_overlay = {1, 1, 1, 0};
};
