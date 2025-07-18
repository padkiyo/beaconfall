#pragma once

#include "core.h"
#include "entity.h"
#include "systems/sprite_system/sprite_system.h"

class Rock : public Entity {
public:
	Rock();
	~Rock();

	// Setters
	void set_pos(const glm::vec2& pos);

	Quad render(const SpriteManager& sprt_mgr);
};
