#include "game_scene.h"

GameScene::GameScene(const GameState& gs)
	: m_gs(gs) {
	m_player = new Player(m_entities, *gs.camera);
	m_player->set_pos({200, 300});

	Rock* rock = new Rock(m_entities);
	rock->set_pos({100, 100});
	m_entities.push_back(rock);

	Rock* rock2 = new Rock(m_entities);
	rock2->set_pos({200, 100});
	m_entities.push_back(rock2);

	m_beacon = new Beacon();
	m_beacon->set_pos({400, 300});
	m_entities.push_back(m_beacon);

	m_beacon_light = {
		{ 400, 300 },
		1,
		0.8,
		glm::radians(0.0),
		glm::radians(360.0 * 2),
		0.5f,
		{ 0, 1, 1, 1 }
	};
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

	// Camera following the player
	Rect r = m_player->rect();
	glm::vec2 to_follow = { r.x, r.y };
	glm::vec3 cam_pos = m_gs.camera->get_pos();
	cam_pos.x += (to_follow.x - cam_pos.x - WIN_WIDTH / 2.0f) / 10.0f;
	cam_pos.y += (to_follow.y - cam_pos.y - WIN_HEIGHT / 2.0f) / 10.0f;
	m_gs.camera->set_pos(cam_pos);

	// Rendering all entities
	for (auto ent: m_entities) {
		ent->render(*m_gs.sprt_mgr, m_quads);
	}

	// Rendering player
	m_player->update(m_gs.fc->dt());
	m_player->render(*m_gs.sprt_mgr, m_quads);

	add_light(m_beacon_light);
}

void GameScene::on_ui_render(UI& ui) {
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

		ImGui::SeparatorText("Beacon Light");
		ImGui::DragFloat2("Position",(float*)&m_beacon_light.pos, 1.0f, -500.0f, 500.0f);
		ImGui::DragFloat("Radius", &m_beacon_light.radius, 0.01f, 0.0f, 50.0f);
		ImGui::DragFloat("Intensity", &m_beacon_light.intensity, 0.01f, 0.0f, 10.0f);
		ImGui::SliderAngle("Direction", &m_beacon_light.dir, 0.0f, 720.0f);
		ImGui::SliderAngle("Fov", &m_beacon_light.fov, 0.0f, 360.0f);
		ImGui::DragFloat("Fall Off", &m_beacon_light.fall_off, 0.1f, 0.0f, 10.0f);
		ImGui::ColorEdit4("Color", (float*)&m_beacon_light.color);
	}
}
