#pragma once

#include "core.h"
#include "game.h"

struct Enemy {
	glm::vec3 pos;
	glm::vec2 size;
	f32 react_time;
	f32 curr_time;
	b32 reacted;
};

class SlowmoScene : public Scene {
public:
	SlowmoScene(const GameState& gs);
	~SlowmoScene();

public:
	void on_enter();
	void on_exit();
	void on_update(f64 dt);
	void on_event(SDL_Event event, f64 dt);
	void on_imgui_render();

private:
	std::vector<Enemy> m_enemies;
	b32 m_start_deadeye;
	f32 m_tick;
	u32 m_deadeye_start_time;

	const GameState& m_gs;
};
