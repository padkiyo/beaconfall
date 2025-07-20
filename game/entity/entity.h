#pragma once

#include "core.h"
#include "systems/sprite_system/sprite_system.h"

enum EntityType {
	ENT_PLAYER,
	ENT_ROCK,
	ENT_GEM,
	ENT_BEACON,
	ENT_ZOMBIE,
};

class Entity {
public:
	EntityType type;

public:
	Entity(EntityType type);
	virtual ~Entity();

	// Getters
	inline Rect rect() const { return m_rect; }
	inline f32 health() const { return m_health; }

	// Setters
	void set_pos(const glm::vec2& pos);
	void set_size(const glm::vec2& size);

	void take_damage(f32 dmg);
	b32 is_stunned();

	// Virtual functions
	virtual void render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads);
	virtual void on_damage();

protected:
	Rect m_rect;
	f32 m_health;

	// Handling stun
	f32 m_stun = 0.0f;
	f32 m_stun_timeout = 10.0f;
	f32 m_stun_decay = 0.8f;
};
