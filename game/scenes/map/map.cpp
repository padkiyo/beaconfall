#include "game_state.h"
#include "systems/map_system/map_system.h"

extern GameState gs;
Map map;

void map_entry(void* data) {
	map = map_load("./assets/maps/test_map/map.json", "./assets/maps/test_map/spritesheet.png").unwrap();
}

void map_exit(void*data) {
	log_info("Map scene is exited\n");
}

void map_update(void*data, f64 dt) {

	rp_push_text(gs.quad_rp, gs.font_regular, "Map Scene", glm::vec3(0, 0, 0),
			glm::vec4(1,1,1,1));

	map_render(gs.quad_rp, &map, 1.5f);
}

void map_event(void* data, SDL_Event event, f64 dt) {
}
