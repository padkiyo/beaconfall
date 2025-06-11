#include "game_state.h"
#include "map.h"

extern GameState gs;

void MapScene::on_enter() {
	log_info("Map scene is entered\n");
}

void MapScene::on_exit() {
	log_info("Map scene is exited\n");
}

void MapScene::on_update(f64 dt) {
	rp_push_text(
		gs.quad_rp,
		gs.font_regular,
		"Map Scene",
		glm::vec3(0, 0, 0),
		glm::vec4(1,1,1,1)
	);

	mm_render_current(gs.quad_rp, gs.mm, 1.0f);
}

void MapScene::on_event(SDL_Event event, f64 dt) {
}

void MapScene::on_imgui_render() {
}
