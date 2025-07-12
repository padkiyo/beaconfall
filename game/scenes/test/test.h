#pragma once

#include "core.h"
#include "game.h"
#include "systems/map_system/map_system.h"
#include "config.h"

class TestScene : public Scene {
public:
	TestScene(const GameState& gs);
	~TestScene();

public:
	void on_enter();
	void on_exit();
	void on_update(f64 dt);
	void on_event(const SDL_Event& event, f64 dt);
	void on_imgui_render();

private:
	const GameState& gs;

	Map* map;

	Light l1, l2;
	glm::vec3 ambient_color = { 0.5, 0.5, 0.5 };
	f32 pixel_size = 2;

	Rect a;
	std::vector<Rect> boxes;
	glm::vec2 move = glm::vec2(0);
	f32 move_speed = 420.0f;
};
