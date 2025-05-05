#include "notebook.h"
#include "game_state.h"
#include "config.h"

extern GameState gs;

void notebook_entry(void* data) {
}

void notebook_exit(void* data) {
}

void notebook_update(void* data, f64 dt) {
	std::string text_1 = "Press Space to open notebook";
	std::string text_2 = "Press Escape to close notebook";
	std::string text_3 = "Press Arrow keys to change page";

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

void notebook_event(void* data, SDL_Event event, f64 dt) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
			case SDLK_SPACE:
				gs.ns->open = true;
				break;
		}
	}
}
