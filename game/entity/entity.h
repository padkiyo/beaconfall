#pragma once

#include "core.h"
#include "systems/sprite_system/sprite_system.h"

enum EntityID {
	ENT_PLAYER,
	ENT_ROCK,
};

class Entity {
public:
	EntityID id;

public:
	Entity(EntityID id);
	virtual ~Entity();

	// Getters
	inline Rect rect() const { return m_rect; }

	// Setters
	void set_pos(const glm::vec2& pos);
	void set_size(const glm::vec2& size);

	// Virtual functions
	virtual Quad render(const SpriteManager& sprt_mgr);

protected:
	Rect m_rect;
};
