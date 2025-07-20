#pragma once

#include "core.h"
#include "systems/sprite_system/sprite_system.h"
#include "entity/entity.h"
#include "game.h"

#define POWER_CONSUMPTION_RATE 5.0f // per s
#define START_POWER 100.0f
#define POWER_INCREASE_BY 50.0f;
#define START_LEVEL 1
#define LEVEL_MAX_XP 100.0f
#define START_RADIUS 250.0f
#define RADIUS_INCREASE_BY 50.0f

class Beacon : public Entity {
public:
	Beacon(const GameState& gs);
	~Beacon();

	f32 get_power();
	inline f32 get_max_power() const { return this->max_power; }
	void add_power(f32 value);
	void set_power(f32 p) { power = p; }

	f32 get_exp();
	void add_exp(f32 value);

	i32 get_level();
	inline f32 get_radius() const { return radius; }
	inline Light get_light() const { return m_light; }
	f32 get_healing();

	void render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads, std::vector<Light>& lights);

public:
	f32 start_time;

private:
	const GameState& m_gs;
	Light m_light;

	// Stats
	i32 level;
	f32 exp;
	f32 max_power;
	f32 power;
	f32 radius;
};
