#include "game_scene.h"

#define MAX_ROCK_COUNT 100

void GameScene::gen_resources(f32& btime, f32 etime) {
		if(((f32) SDL_GetTicks() - btime) >= (etime*1000)){
			btime = (f32) SDL_GetTicks();

			i32 randomxc = rand() % 2;
			i32 randomx;

			switch(randomxc) {
				case 0:
					randomx = rand() % 9;
					break;
				case 1:
					randomx = rand() % 9 + 11;
					break;
			}

			i32 randomyc = rand() % 2;
			i32 randomy;

			switch(randomyc) {
				case 0:
					randomy = rand() % 8;
					break;
				case 1:
					randomy = rand() % 8 + 9;
			}
			Rock* rock = new Rock(m_entities);

			randomx = (randomx*64) - 274;
			randomy = (randomy*64) -212;

			rock->set_pos({randomx, randomy});
			m_entities.push_back(rock);
		}
}

GameScene::GameScene(const GameState& gs)
	: m_gs(gs) {
	m_player = new Player(m_entities);
	m_player->set_pos({200, 300});

	Rock* rock = new Rock(m_entities);
	rock->set_pos({100, 100});
	m_entities.push_back(rock);

	Rock* rock2 = new Rock(m_entities);
	rock2->set_pos({200, 100});
	m_entities.push_back(rock2);


	beacon = new Beacon();
	beacon->set_pos({350, 250});
	m_entities.push_back(beacon);

	zombie = new Zombie(m_player, beacon, m_entities, m_gs.fc);
	zombie->set_pos({200, 150});
	m_entities.push_back(zombie);
}

GameScene::~GameScene() {
	for (auto ent: m_entities) {
		delete ent;
	}
}

void GameScene::on_enter() {
	log_info("Game scene is entered\n");
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
	map_texture.bind();
	add_quad(
			Quad {
			{400-674, 300-512, 0},
			{674 * 2, 512 * 2},
			glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), {0,0,1}),
			&map_texture,
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			{1, 1, 1, 1}
			}
			);

	this->gen_resources(this->begin_time, 10.0f);

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

	if (ImGui::CollapsingHeader("Beacon")) {
		ImGui::Text("Beacon Power: %f", beacon->get_power());
		ImGui::Text("Beacon Exp: %f", beacon->get_exp());
		ImGui::Text("Beacon Level: %d", beacon->get_level());

		if(ImGui::Button("+10 Power")){
			beacon->add_power(10.0f);
		}
		if(ImGui::Button("+10 Exp")) {
			beacon->add_exp(10.0f);
		}
	}
}
