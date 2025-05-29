#pragma once

#include "core.h"

struct Enemy {
	glm::vec3 pos;
	glm::vec2 size;
	f32 react_time;
	f32 curr_time;
	b32 reacted;
};

struct Slowmo {
	std::vector<Enemy> enemies;
	b32 start_deadeye;
	f32 tick;
	u32 deadeye_start_time;
};

Slowmo* slowmo_create();

void slowmo_entry(void* data);
void slowmo_exit(void* data);
void slowmo_update(void* data, f64 dt);
void slowmo_event(void* data, SDL_Event event, f64 dt);
