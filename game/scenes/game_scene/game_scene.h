#pragma once

#include "core.h"
#include "game.h"
#include "config.h"
#include "entity/player.h"
#include "entity/rock.h"
#include "structure/beacon.h"
#include "entity/zombie.h"


#define INIT_SPAWN_RATE_ZOMBIE 20.0f
#define INIT_SPAWN_RATE_RESOURCES 2.0f

#define SPAWN_RATE_INC_ZOMBIE 0.5f

class GameScene : public Scene {
public:
	GameScene(const GameState& gs);
	~GameScene();

public:
	void on_enter();
	void on_exit();
	void on_event(const SDL_Event& event, f64 dt);
	void on_update(f64 dt);
	void on_ui_render(UI& ui);
	void on_imgui_render();

private:
	void gen_resources(f32& btime, f32 etime);
	void gen_zombies(f32& btime, f32 etime);

private:
	i32 m_prev_night = 0;
	f32 zombie_spawn_rate = INIT_SPAWN_RATE_ZOMBIE;
	f32 resources_spawn_rate = INIT_SPAWN_RATE_RESOURCES;

	const GameState& m_gs;

	std::vector<Rect> m_boxes;

	// Player
	Player* m_player;

	// Beacon
	Beacon* m_beacon;

	// Entities
	std::vector<Entity*> m_entities;

	// Light
	glm::vec3 ambient_color = { 0.5, 0.5, 0.5 };
	f32 pixel_size = 2;

	TextureFilter sprite_filter = {
		.min_filter = GL_NEAREST,
		.mag_filter = GL_NEAREST,
		.wrap_s = GL_CLAMP_TO_EDGE,
		.wrap_t = GL_CLAMP_TO_EDGE,
		.flip = true
	};

	Zombie* zombie;

	Texture map_texture = Texture("./assets/MAP.png", sprite_filter);

	f32 begin_time = SDL_GetTicks();


	// Day Night cycle
	f32 m_start_time;
	f32 m_cycle_time = 10.0f; // In seconds
	b32 m_cycle_complete = false;
	b32 m_is_day = true;
	f32 m_day_color = 0.8f;
	i32 m_night_count = 0;
};

