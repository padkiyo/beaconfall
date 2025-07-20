#include "game_scene.h"

GameScene::GameScene(const GameState& gs)
	: m_gs(gs) {
	m_player = new Player(m_entities, *gs.camera);
	m_player->set_pos({0, 100});

	Rock* rock = new Rock(m_entities);
	rock->set_pos({400, 300});
	m_entities.push_back(rock);

	Rock* rock2 = new Rock(m_entities);
	rock2->set_pos({300, 400});
	m_entities.push_back(rock2);

	m_beacon = new Beacon();
	m_beacon->set_pos({0, 0});
	m_entities.push_back(m_beacon);
}

GameScene::~GameScene() {
	for (auto ent: m_entities) {
		delete ent;
	}
}

void GameScene::on_enter() {
	log_info("Game scene is entered\n");

	m_start_time = SDL_GetTicks() / 1000.0f;
	m_is_day = true;
	m_day_color = 0.8f;
}

void GameScene::on_exit() {
	log_info("Game scene is exited\n");
}

void GameScene::on_event(const SDL_Event& event, f64 dt) {
	m_player->handle_event(event);
}

void GameScene::on_update(f64 dt) {
	f32 end_time = SDL_GetTicks() / 1000.0f;
	f32 time_passed = end_time - m_start_time;

	// Calculating day night ambience color
	if (m_is_day) {
		m_day_color = (m_cycle_time - time_passed) / m_cycle_time;
	} else {
		m_day_color = time_passed / m_cycle_time;
	}

	if (m_day_color >= 0.8) m_day_color = 0.8;
	if (m_day_color <= 0.3) m_day_color = 0.3;
	ambient_color = { m_day_color, m_day_color, m_day_color };

	// Switching day and night
	if (time_passed >= m_cycle_time) {
		m_start_time = SDL_GetTicks() / 1000.0f;
		m_is_day = !m_is_day;
	}

	// Setting up the lights
	set_ambient_color(ambient_color);
	set_light_pixel_size({pixel_size, pixel_size});

	// Camera following the player
	Rect r = m_player->rect();
	glm::vec2 to_follow = { r.x, r.y };
	glm::vec3 cam_pos = m_gs.camera->get_pos();
	cam_pos.x += (to_follow.x - cam_pos.x - WIN_WIDTH / 2.0f) / 10.0f;
	cam_pos.y += (to_follow.y - cam_pos.y - WIN_HEIGHT / 2.0f) / 10.0f;
	m_gs.camera->set_pos(cam_pos);

	// Rendering all entities
	for (auto ent: m_entities) {
		ent->render(*m_gs.sprt_mgr, m_quads, m_lights);
	}

	// Rendering player
	m_player->update(m_beacon, m_gs.fc->dt());
	m_player->render(*m_gs.sprt_mgr, m_quads, m_lights);
}

void GameScene::on_ui_render(UI& ui) {
	ui.progress_bar(
		Rect({10, WIN_HEIGHT - 30, 100, 20}),
		m_player->health(), 100.0f,
		Style { .fg_color = { 0, 1, 0.3, 1 }}
	);
	ui.progress_bar(
		Rect({10, WIN_HEIGHT - 60, 100, 20}),
		m_beacon->get_power(), m_beacon->get_max_power(),
		Style { .fg_color = { 0, 1, 1, 1 }}
	);
}

void GameScene::on_imgui_render() {
	if (ImGui::CollapsingHeader("Beacon")) {
		ImGui::Text("Beacon Power: %f", m_beacon->get_power());
		ImGui::Text("Beacon Exp: %f", m_beacon->get_exp());
		ImGui::Text("Beacon Level: %d", m_beacon->get_level());

		if(ImGui::Button("+10 Power")){
			m_beacon->add_power(10.0f);
		}
		if(ImGui::Button("+10 Exp")) {
			m_beacon->add_exp(10.0f);
		}
	}

	if (ImGui::CollapsingHeader("Light")) {
		ImGui::ColorEdit3("Ambient Color", (f32*) &ambient_color);
		ImGui::DragFloat("Pixel Size", &pixel_size, 0.1f, 1.0f, 50.0f);
	}
}
