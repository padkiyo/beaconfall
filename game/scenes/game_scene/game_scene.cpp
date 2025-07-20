#include "game_scene.h"
#include "scenes/scenes.h"

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


void GameScene::gen_zombies(f32& btime, f32 etime) {
		if(((f32) SDL_GetTicks() - btime) >= (etime*1000)){
			btime = (f32) SDL_GetTicks();

			i32 randomxc = rand() % 2;
			i32 randomx;

			switch(randomxc) {
				case 0:
					randomx = rand() % 11;
					break;
				case 1:
					randomx = rand() % 11 + 14;
					break;
			}

			i32 randomyc = rand() % 2;
			i32 randomy;

			switch(randomyc) {
				case 0:
					randomy = rand() % 10;
					break;
				case 1:
					randomy = rand() % 10 + 11;
			}
			Zombie* zom = new Zombie(m_player, m_beacon, m_entities, m_gs.fc);

			randomx = (randomx*64) - 274;
			randomy = (randomy*64) -212;

			zom->set_pos({randomx, randomy});
			m_entities.push_back(zom);
		}
}

GameScene::GameScene(const GameState& gs)
	: m_gs(gs) {
	m_player = new Player(m_entities, *gs.camera, gs);
	m_player->set_pos({0, 100});

	Rock* rock = new Rock(m_entities, gs);
	rock->set_pos({400, 300});
	m_entities.push_back(rock);

	Rock* rock2 = new Rock(m_entities, gs);
	rock2->set_pos({300, 400});
	m_entities.push_back(rock2);

	m_beacon = new Beacon(gs);
	m_beacon->set_pos({350, 250});
	m_entities.push_back(m_beacon);

	zombie = new Zombie(m_player, m_beacon, m_entities, m_gs.fc);
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

	m_start_time = SDL_GetTicks() / 1000.0f;
	m_is_day = true;
	m_day_color = 0.8f;
	m_night_count = 0;
	m_cycle_complete = false;
	m_beacon->start_time = SDL_GetTicks();
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

	std::cout << m_night_count << std::endl;

	this->zombie_spawn_rate = INIT_SPAWN_RATE_ZOMBIE -  m_night_count;

	// Calculating day night ambience color
	if (!m_cycle_complete) {
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
		m_cycle_complete = !m_cycle_complete;
		m_night_count++;
	}

	if (!m_cycle_complete) {
		if (time_passed / m_cycle_time >= 0.8f) {
			m_is_day = false;
		}
	} else {
		if (time_passed / m_cycle_time >= 0.2f) {
			m_is_day = true;
			m_gs.snow_sys->storm(false);
		}
	}

	// Enabling storm
	if (!m_is_day && m_night_count > 2) {
		if (rand_range(0, 10) < m_night_count) {
			m_gs.snow_sys->storm(true);
		}
	}

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

	this->gen_resources(this->begin_time, this->resources_spawn_rate);

	if(!m_is_day) {
		this->gen_zombies(this->begin_time, this->zombie_spawn_rate);
	}

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


	// DIE
	if (m_player->health() <= 0.0f) {
		m_gs.scene_mgr->switch_scene(SCENE_MENU);
	}
}

void GameScene::on_ui_render(UI& ui) {
		std::string beacon_level_lable = std::format("BEACON LEVEL: {}", std::to_string(m_beacon->get_level()));
    glm::vec2 cnt_size = m_gs.font_regular->calculate_dim(beacon_level_lable);
    ui.text(beacon_level_lable, {WIN_WIDTH - cnt_size.x - 10, WIN_HEIGHT - cnt_size.y - 50}, Style {
        .font = m_gs.font_regular,
    });

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

	std::string cnt_label = std::format("Night: {}", std::to_string(m_night_count));
	glm::vec2 cnt_size = m_gs.font_regular->calculate_dim(cnt_label);
	ui.text(cnt_label, {WIN_WIDTH - cnt_size.x - 10, WIN_HEIGHT - cnt_size.y - 10}, Style {
		.font = m_gs.font_regular,
	});
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
