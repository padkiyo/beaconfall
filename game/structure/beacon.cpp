#include "beacon.h"
Beacon::Beacon() {
}

Beacon::Beacon(glm::vec2 pos, glm::vec2 size) {
	this->pos = pos;
	this->size = size;

	this->level = START_LEVEL;
	this->exp = 0.0f;
	this->power = START_POWER;
}

f32 Beacon::get_power() {
	return this->power;
}

void Beacon::add_power(f32 value) {
	this->power += value;

	if (this->power >= 100.0f) {
		this->power = 100.0f;
	}
}

void Beacon::add_exp(f32 value) {
	this->exp += value;

	if (this->exp >= LEVEL_MAX_XP) {
		this->exp = 0.0f;
		this->level += 1;

		if (this->level >= 3)
		{
			this->level = 3;
		}
	}
}

f32 Beacon::get_exp(){
	return this->exp;
}

i32 Beacon::get_level() {
	return this->level;
}

Quad Beacon::render(SpriteManager& sm ) {
	sm.activate_spritesheet(BEACON);

	f32 dt = (SDL_GetTicks() - this->start_time);

	this->power -= (POWER_CONSUMPTION_RATE/ 1000.0f) * dt;

	this->start_time = SDL_GetTicks();

	return Quad {
		{pos.x, pos.y, 0},
		{size.x, size.y},
		glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), {0,0,1}),
		&sm.get_spritesheet_texture(BEACON),
		sm.get_frame(BEACON_DEFAULT)[this->level - 1],
		{1, 1, 1, 1}
	};
}
