#include "beacon.h"

Beacon::Beacon()
	: Entity(ENT_BEACON) {
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = 100;
	m_rect.h = 100;

	this->level = START_LEVEL;
	this->exp = 0.0f;
	this->power = START_POWER;
}

Beacon::~Beacon() {
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

void Beacon::render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads) {
	f32 dt = (SDL_GetTicks() - this->start_time);

	this->power -= (POWER_CONSUMPTION_RATE/ 1000.0f) * dt;

	this->start_time = SDL_GetTicks();

	quads.push_back(Quad {
		{m_rect.x, m_rect.y, 0},
		{m_rect.w, m_rect.h},
		glm::mat4(1),
		&sprt_mgr.get_spritesheet_texture(BEACON),
		sprt_mgr.get_frame(BEACON_DEFAULT)[this->level - 1],
		{1, 1, 1, 1}
	});
}
