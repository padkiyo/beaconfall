#include "dialog.h"
#include "config.h"
#include "game_state.h"

extern GameState gs;

void dialog_entry(void* data) {
	log_info("Dialog scene is entered\n");
}

void dialog_exit(void* data) {
	log_info("Dialog scene is exited\n");
}

void dialog_update(void* data, f64 dt) {
	std::string text = "Hello world";
	glm::vec3 pos = { 100, 100, 0 };
	glm::vec2 size = font_calc_size(gs.font_regular, text);
	rp_push_quad(gs.quad_rp, pos, size, glm::vec4(1, 0, 0, 1), gs.quad_rp->white_texture.id, glm::vec4(0, 0, 1, 1));
	rp_push_text(gs.quad_rp, gs.font_regular, text, pos, glm::vec4(1, 1, 1, 1));
}

void dialog_event(void* data, SDL_Event event, f64 dt) {
	log_info("Event in Dialog scene\n");
}
