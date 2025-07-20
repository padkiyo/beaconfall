#pragma once

#include "core.h"
#include "game.h"
#include "config.h"
#include "entity/player.h"
#include "entity/rock.h"
#include "structure/beacon.h"

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
	const GameState& m_gs;

	std::vector<Rect> m_boxes;

	// Player
	Player* m_player;

	// Beacon
	Beacon* m_beacon;
	Light m_beacon_light;

	// Entities
	std::vector<Entity*> m_entities;

	// Light
	glm::vec3 ambient_color = { 0.5, 0.5, 0.5 };
	f32 pixel_size = 2;
};

