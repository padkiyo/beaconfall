#pragma once

#include "core.h"
#include "systems/sprite_system/sprite_system.h"
#include "entity.h"
#include "config.h"
#include "structure/beacon.h"
#include "item/gem.h"
#include "game.h"

class Player : public Entity {
public:
	Player(std::vector<Entity*>& entities, const Camera& camera, const GameState& gs);
	~Player();

	// Setters
	void set_speed(f32 speed);

	void handle_event(const SDL_Event& event);
	void update(Beacon* beacon, f64 dt);
	void render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads, std::vector<Light>& lights);

private:
	void handle_attack();
	void handle_collision(f64 dt);
	void handle_item_pickup();
	void handle_beacon_interaction(Beacon* beacon);
	void calc_atk_hitbox(i32 x, i32 y);

private:
	const GameState& m_gs;

	// Interactions
	Rect m_reach_area;
	b32 m_feed_beacon;

	// Pickup
	Entity* m_picked_item;

	// Movement
	glm::vec2 m_move;
	f32 m_speed;
	b32 m_left, m_right, m_up, m_down;

	// Attack
	b32 m_can_atk;
	b32 m_attack;
	f32 m_atk_dmg;
	Rect m_atk_hitbox;
	f32 m_atk_cooldown;

	glm::vec4 m_overlay = { 1, 1, 1, 0 };

	std::vector<Entity*>& m_entities;
	const Camera& m_camera;
};
