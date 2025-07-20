#pragma once

#include "core.h"

#define SPAWN_TIMEOUT 10.0f
#define SPAWN_DECAY 0.2f
#define STORM_SPAWN_DECAY 0.5f
#define SNOW_DECAY 0.1f
#define SNOW_DIR_TIMEOUT 5.0f
#define SNOW_DIR_DECAY 1.0f
#define STORM_SPEED 300.0f
#define NORMAL_SPEED 100.0f

#define STORM_DMG 0.5f
#define NORMAL_DMG 0.2f


struct Snow {
	glm::vec2 pos;
	glm::vec2 size = { 5, 5 };
	f32 dir = 0;
	f32 dir_change = 0.0f;
	f32 speed;
	f32 lifetime;
};

class SnowSystem {
public:
	SnowSystem(const glm::vec2& res);
	~SnowSystem();

	void add_spawn_point(const glm::vec2& point);
	inline void storm(b32 s) { m_storm = s; }
	inline b32 is_storm() const { return m_storm; }
	inline f32 get_damage() const {
		if (m_storm) return STORM_DMG;
		return NORMAL_DMG;
	}
	void spawn();
	void update(Renderer* renderer, f64 dt);

private:
	glm::vec2 m_res;
	std::vector<glm::vec2> m_spawn_points;
	std::vector<Snow> m_particals;

	f32 m_spawn_timeout = 0;
	b32 m_storm = false;

	Shader* m_shader;
	Camera* m_camera;
};
