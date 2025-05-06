#include "game_state.h"
#include "systems/map_system/map_system.h"

extern GameState gs;
Map map;

void map_entry(void* data) {
	log_info("Map scene is entered\n");
}

void map_exit(void*data) {
	log_info("Map scene is exited\n");
}

void map_update(void*data, f64 dt) {

	rp_push_text(gs.quad_rp, gs.font_regular, "Map Scene", glm::vec3(0, 0, 0),
			glm::vec4(1,1,1,1));

	mm_render_current(gs.quad_rp, gs.mm, 3.0f);
}

void map_event(void* data, SDL_Event event, f64 dt) {
}
