#include "dialog.h"
#include "game_state.h"

extern GameState gs;

void dialog_entry(void* data) {
	log_info("Dialog scene is entered\n");
}

void dialog_exit(void* data) {
	log_info("Dialog scene is exited\n");
}

void dialog_update(void* data, f64 dt) {
	// rp_push_quad(gs.quad_rp, glm::vec3(0, 0, 0), glm::vec2(50, 50), glm::vec4(1, 0, 0, 1), gs.quad_rp->white_texture.id, glm::vec4(0, 0, 1, 1));
	rp_push_text(gs.quad_rp, gs.font_regular, "Hello world", glm::vec3(100, 100, 0), glm::vec4(1, 1, 1, 1));
}

void dialog_event(void* data, SDL_Event event, f64 dt) {
	log_info("Event in Dialog scene\n");
}
