#include "beacon_scene.h"

BeaconScene::BeaconScene(const GameState& gs)
	: m_gs(gs) {
	beacon = new Beacon();
	beacon->set_pos({400, 300});
}

BeaconScene::~BeaconScene() {
}

void BeaconScene::on_enter() {
	log_info("Game scene is entered\n");
}

void BeaconScene::on_exit() {
	log_info("Game scene is exited\n");
}

void BeaconScene::on_event(const SDL_Event& event, f64 dt) {
}

void BeaconScene::on_update(f64 dt) {
	// Setting up the lights
	set_ambient_color(ambient_color);
	set_light_pixel_size({pixel_size, pixel_size});

	beacon->render(*m_gs.sprt_mgr, m_quads, m_lights);
}

void BeaconScene::on_ui_render(UI& ui) {
}

void BeaconScene::on_imgui_render() {
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
