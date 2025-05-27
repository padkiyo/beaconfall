#include "slowmo.h"
#include "game_state.h"
#include "config.h"

#define DEAD_EYE_TIME 3000
#define ENEMY_REACTION_TIME 3
#define TICK 1
#define SLOWMO_TICK 0.1

extern GameState gs;

Slowmo* slowmo_create() {
	Slowmo* scene = new Slowmo;

	scene->start_deadeye = false;
	scene->tick = TICK;
	scene->enemies = {
		{
			glm::vec3(100, 200, 0),
			2,
			0,
			false,
		},
		{
			glm::vec3(300, 200, 0),
			1,
			0,
			false,
		},
		{
			glm::vec3(500, 200, 0),
			3,
			0,
			false,
		},
	};

	return scene;
}

void slowmo_entry(void* data) {
}

void slowmo_exit(void* data) {
}

void slowmo_update(void* data, f64 dt) {
	Slowmo* scene = reinterpret_cast<Slowmo*>(data);

	std::string text_1 = "Slowmo Scene";
	std::string text_2 = "Press Space to enter Dead Eye";
	std::string text_3 = "Press Escape to reset Dead Eye";

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

	// TODO:
	// When peaked start the reaction timer
	// The slowmo starts
	// The slowmo has limited amount of time
	// When slomo stops the reaction timer moves faster
	// END

	if (scene->start_deadeye) {

		u32 deadeye_time_elapsed = SDL_GetTicks() - scene->deadeye_start_time;
		f32 progress = 100.0f - (((f32) deadeye_time_elapsed / (f32) DEAD_EYE_TIME) * 100.0f);

		if (progress <= 0.0f)
			progress = 0.0f;

		rp_push_quad(
			gs.quad_rp,
			glm::vec3(50, WIN_HEIGHT - 100, 0),
			glm::vec2(progress, 20),
			glm::vec4(0.5, 0.5, 0, 1),
			gs.quad_rp->white_texture.id,
			glm::vec4(0, 0, 1, 1)
		);

		// Switching to normal tick when dead eye effect is over
		if (deadeye_time_elapsed >= DEAD_EYE_TIME) {
			scene->tick = TICK;
		}

		// Updating the enemies reaction time
		for (auto& enemy : scene->enemies) {
			enemy.curr_time += ENEMY_REACTION_TIME * dt * scene->tick;

			if (enemy.react_time <= enemy.curr_time) {
				enemy.reacted = true;
			}
		}
	}

	// Rendering the enemies
	for (auto& enemy : scene->enemies) {
		if (enemy.reacted) {
			rp_push_quad(
				gs.quad_rp,
				enemy.pos,
				glm::vec2(50, 100),
				glm::vec4(1, 0, 0, 1),
				gs.quad_rp->white_texture.id,
				glm::vec4(0, 0, 1, 1)
			);
		} else {
			rp_push_quad(
				gs.quad_rp,
				enemy.pos,
				glm::vec2(50, 100),
				glm::vec4(
					1 - ((enemy.react_time - enemy.curr_time) / enemy.react_time),
					0, 0, 1
				),
				gs.quad_rp->white_texture.id,
				glm::vec4(0, 0, 1, 1)
			);
		}
	}
}

void slowmo_event(void* data, SDL_Event event, f64 dt) {
	Slowmo* scene = reinterpret_cast<Slowmo*>(data);

	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {

			// Starting the deadeye
			case SDLK_SPACE:
				scene->start_deadeye = true;
				scene->tick = SLOWMO_TICK;
				scene->deadeye_start_time = SDL_GetTicks();
				break;

			// Reset the deadeye
			case SDLK_ESCAPE: {
				scene->start_deadeye = false;
				scene->tick = TICK;
				scene->deadeye_start_time = SDL_GetTicks();

				for (auto& enemy : scene->enemies) {
					enemy.curr_time = 0;
					enemy.reacted = false;
				}

			} break;
		}
	}
}
