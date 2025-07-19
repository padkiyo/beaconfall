#pragma once

#include "core.h"
#include "systems/sprite_system/sprite_system.h"
#include "entity/entity.h"

#define POWER_CONSUMPTION_RATE 1.0f // per s
#define START_POWER 100.0f
#define START_LEVEL 1
#define LEVEL_MAX_XP 100.0f

class Beacon : public Entity {
	private:
		i32 level;
		f32 exp;
		f32 power;
		f32 radius;

		f32 start_time = SDL_GetTicks();

	public:
		// For initializing shit
		Beacon();

		f32 get_power();
		void add_power(f32 value);

		f32 get_exp();
		void add_exp(f32 value);

		i32 get_level();

		void render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads);
};
