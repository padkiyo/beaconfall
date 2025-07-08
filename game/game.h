#pragma once

#include "core.h"


/*
 * This is the global state of the game.
 * This includes all the things that part of games require
 * The lifetime of these variables will be throught the game lifetime
 */

struct GameState {
	// Core
	Window* window;
	Renderer* renderer;
	AudioManager* audio_mgr;
	Camera* camera;
	SceneManager* scene_mgr;
	FrameController* fc;

	// Resources
	Font* font_regular;
};


/*
 * This is the entry point of our game.
 * Every game logic and resources are gonna be inside of this class
 */

class Game {
public:
	Game();
	~Game();
	void run();

private:
	void event();
	void render();
	void overlay();
	void imgui_render();

private:
	void init_core();
	void init_scenes();
	void init_systems();
	void init_resources();
	void use_resources();

private:
	GameState m_gs;
	b32 m_running;

	Light l1, l2;
	glm::vec3 ambient_color = { 0.5, 0.5, 0.5 };
	f32 pixel_size = 2;

	Rect a;
	std::vector<Rect> boxes;
	glm::vec2 move = glm::vec2(0);
	f32 move_speed = 420.0f;
};
