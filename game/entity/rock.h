#pragma once

#include "core.h"
#include "entity.h"
#include "systems/sprite_system/sprite_system.h"
#include "game.h"

class Rock : public Entity {
public:
	Rock(std::vector<Entity*>& entities, const GameState& gs);
	~Rock();

	void on_damage();
	void render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads, std::vector<Light>& lights);

private:
	const GameState& m_gs;
	std::vector<Entity*>& m_entities;
	glm::vec4 m_overlay = {1, 1, 1, 0};
};
