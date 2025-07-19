#include "beacon_scene.h"

BeaconScene::BeaconScene(const GameState& gs)
	: m_gs(gs) {
}

BeaconScene::~BeaconScene() {
}

void BeaconScene::on_enter() {
	log_info("Game scene is entered\n");

	m_gs.player->set_pos({400, 300});

	Sprite beacon_sprite = {
		.path="./assets/beacons.png",
		.x_cnt=3,
		.y_cnt=1
	};

	m_gs.sprt_mgr->add_sprite(beacon_sprite,BEACON);
	m_gs.sprt_mgr->create_frame(BEACON, 0, 2,BEACON_DEFAULT);

	beacon = Beacon(glm::vec2(400,300), glm::vec2(100,100));

}

void BeaconScene::on_exit() {
	log_info("Game scene is exited\n");
}

void BeaconScene::on_event(const SDL_Event& event, f64 dt) {
	m_gs.player->handle_event(event);
}

void BeaconScene::on_update(f64 dt) {
	// Setting up the lights
	set_ambient_color(ambient_color);
	set_light_pixel_size({pixel_size, pixel_size});

	map_texture.bind();
	add_quad(
			Quad {
				{300, 400, 0},
				{480, 416},
				glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), {0,0,1}),
				&map_texture,
				glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
				{1, 1, 1, 1}
			}
	);
	add_quad(beacon.render(*m_gs.sprt_mgr));
	m_gs.player->update(m_boxes, m_gs.fc->dt());

	Quad quad = m_gs.player->render();
	add_quad(quad);

}

void BeaconScene::on_ui_render(UI& ui) {
}

void BeaconScene::on_imgui_render() {
	if(ImGui::CollapsingHeader("Beacon")) {
		ImGui::Text("Beacon Power: %f", beacon.get_power());
		ImGui::Text("Beacon Exp: %f", beacon.get_exp());
		ImGui::Text("Beacon Level: %d", beacon.get_level());

		if(ImGui::Button("+10 Power")){
			beacon.add_power(10.0f);
		}
		if(ImGui::Button("+10 Exp")) {
			beacon.add_exp(10.0f);
		}
	}

}
