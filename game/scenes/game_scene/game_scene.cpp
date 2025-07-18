#include "game_scene.h"

GameScene::GameScene(const GameState& gs)
	: m_gs(gs) {
}

GameScene::~GameScene() {
}

void GameScene::on_enter() {
	log_info("Game scene is entered\n");
	m_gs.player->set_pos({400, 300});
}

void GameScene::on_exit() {
	log_info("Game scene is exited\n");
}

void GameScene::on_event(const SDL_Event& event, f64 dt) {
	m_gs.player->handle_event(event);
}

void GameScene::on_update(f64 dt) {
	// Setting up the lights
	set_ambient_color(ambient_color);
	set_light_pixel_size({pixel_size, pixel_size});

	m_gs.player->update(m_boxes, m_gs.fc->dt());
	Quad quad = m_gs.player->render();
	add_quad(quad);
}

void GameScene::on_ui_render(UI& ui) {
}

void GameScene::on_imgui_render() {
}
