#include "game_scene.h"

GameScene::GameScene(const GameState& gs)
	: m_gs(gs) {
	m_player = new Player(m_entities);
	m_player->set_pos({400, 300});

	Rock* rock = new Rock(m_entities);
	rock->set_pos({100, 100});
	m_entities.push_back(rock);

	Rock* rock2 = new Rock(m_entities);
	rock2->set_pos({200, 100});
	m_entities.push_back(rock2);
}

GameScene::~GameScene() {
}

void GameScene::on_enter() {
	log_info("Game scene is entered\n");
	m_player->set_pos({400, 300});
}

void GameScene::on_exit() {
	log_info("Game scene is exited\n");
}

void GameScene::on_event(const SDL_Event& event, f64 dt) {
	m_player->handle_event(event);
}

void GameScene::on_update(f64 dt) {
	// Setting up the lights
	set_ambient_color(ambient_color);
	set_light_pixel_size({pixel_size, pixel_size});

	// Rendering all entities
	for (auto ent: m_entities) {
		ent->render(*m_gs.sprt_mgr, m_quads);
	}

	// Rendering player
	m_player->update(m_gs.fc->dt());
	m_player->render(*m_gs.sprt_mgr, m_quads);
}

void GameScene::on_ui_render(UI& ui) {
}

void GameScene::on_imgui_render() {
}
