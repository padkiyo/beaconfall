#include "dialog.h"
#include "config.h"
#include "game_state.h"
#include "systems/dialog_system/dialogs.h"

extern GameState gs;

void dialog_entry(void* data) {
	log_info("Dialog scene is entered\n");
}

void dialog_exit(void* data) {
	dialog_system_end_dialog(gs.ds);
	log_info("Dialog scene is exited\n");
}

void dialog_update(void* data, f64 dt) {
	std::string text_1 = "Press Enter to start dialog";
	std::string text_2 = "Press Space to progress dialog";
	std::string text_3 = "Press Escape to skip dialog";

	glm::vec2 size_1 = font_calc_size(gs.font_regular, text_1);
	glm::vec2 size_2 = font_calc_size(gs.font_regular, text_2);
	glm::vec2 size_3 = font_calc_size(gs.font_regular, text_3);

	glm::vec3 pos_1 = {WIN_WIDTH / 2 - size_1.x / 2, 50, 0};
	glm::vec3 pos_2 = {WIN_WIDTH / 2 - size_2.x / 2, pos_1.y + size_1.y + 10, 0};
	glm::vec3 pos_3 = {WIN_WIDTH / 2 - size_3.x / 2, pos_2.y + size_2.y + 10, 0};

	rp_push_text(
		gs.quad_rp,
		gs.font_regular,
		text_1,
		pos_1,
		glm::vec4(1, 1, 1, 1)
	);

	rp_push_text(
		gs.quad_rp,
		gs.font_regular,
		text_2,
		pos_2,
		glm::vec4(1, 1, 1, 1)
	);

	rp_push_text(
		gs.quad_rp,
		gs.font_regular,
		text_3,
		pos_3,
		glm::vec4(1, 1, 1, 1)
	);
}

void dialog_event(void* data, SDL_Event event, f64 dt) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
			case SDLK_RETURN:
				log_info("Starting test dialog\n");
				dialog_system_start_dialog(gs.ds, DIALOG_TEST);
				break;
		}
	}
}
