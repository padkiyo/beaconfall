#include "beacon.h"

Beacon::Beacon(const GameState& gs)
	: Entity(ENT_BEACON), m_gs(gs) {
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = 100;
	m_rect.h = 100;

	this->level = START_LEVEL;
	this->exp = 0.0f;
	this->power = START_POWER;
	this->max_power = START_POWER;
	this->radius = START_RADIUS;
}

Beacon::~Beacon() {
}

f32 Beacon::get_power() {
	return this->power;
}

void Beacon::add_power(f32 value) {
	this->power += value;

	if (this->power >= this->max_power) {
		this->power = this->max_power;
	}
}

void Beacon::add_exp(f32 value) {
	this->exp += value;

	if (this->exp >= LEVEL_MAX_XP) {
		this->exp = 0.0f;
		this->level += 1;
		this->radius += RADIUS_INCREASE_BY;
		this->max_power += POWER_INCREASE_BY;

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

f32 Beacon::get_healing() {
	return (max_power / START_POWER) / 2.0f;
}

void Beacon::render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads, std::vector<Light>& lights) {
	// Degrading power wrt time
	f32 dt = (SDL_GetTicks() - this->start_time);
	this->start_time = SDL_GetTicks();

	this->power -= (POWER_CONSUMPTION_RATE/ 1000.0f) * dt;
	if (this->power <= 0.0f) this->power = 0.0f;

	// Degrading power in storm
	if (m_gs.snow_sys->is_storm()) {
		this->power -= (POWER_CONSUMPTION_RATE/ 1000.0f) * dt;
	}

	// Updating the beacon light
	f32 intensity = this->power / this->max_power;
	if (intensity >= 0.9) intensity = 0.9;
	if (intensity <= 0.1) intensity = 0.1;

	m_light = {
		{ m_rect.x + m_rect.w / 2, m_rect.y + m_rect.h / 2 },
		radius,
		intensity,
		glm::radians(0.0),
		glm::radians(360.0 * 2),
		0.5f,
		{ 0, 1, 1, 1 }
	};

	lights.push_back(m_light);

	quads.push_back(Quad {
		{m_rect.x, m_rect.y, 0},
		{m_rect.w, m_rect.h},
		glm::mat4(1),
		&sprt_mgr.get_spritesheet_texture(BEACON),
		sprt_mgr.get_frame(BEACON_DEFAULT)[2],
		{1, 1, 1, 1}
	});
}
