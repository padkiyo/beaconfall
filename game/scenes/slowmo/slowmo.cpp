#include "slowmo.h"
#include "game_state.h"
#include "config.h"

#define DEAD_EYE_TIME 3000
#define ENEMY_REACTION_TIME 3
#define TICK 1
#define SLOWMO_TICK 0.1

extern GameState gs;

b32 intersectRayTriangle(
	glm::vec3 orig, glm::vec3 dir,
	glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
	glm::vec3 &outIntersectionPoint
) {
	const f32 EPSILON = 1e-8;
	glm::vec3 edge1 = v1 - v0;
	glm::vec3 edge2 = v2 - v0;
	glm::vec3 h = glm::cross(dir, edge2);
	f32 a = glm::dot(edge1, h);
	if (a > -EPSILON && a < EPSILON) return false;

	f32 f = 1.0 / a;
	glm::vec3 s = orig - v0;
	f32 u = f * glm::dot(s, h);
	if (u < 0.0 || u > 1.0) return false;

	glm::vec3 q = glm::cross(s, edge1);
	f32 v = f * glm::dot(dir, q);
	if (v < 0.0 || u + v > 1.0) return false;

	f32 t = f * glm::dot(edge2, q);
	if (t > EPSILON) {
			outIntersectionPoint = orig + dir * t;
			return true;
	}

	return false;
}

SlowmoScene::SlowmoScene()
	: m_start_deadeye(false), m_tick(TICK) {
	m_enemies = {
		{
			glm::vec3(-0.5, 0, 1),
			glm::vec2(0.2, 0.5),
			2,
			0,
			false,
		},
		{
			glm::vec3(0, 0, 1),
			glm::vec2(0.2, 0.5),
			1,
			0,
			false,
		},
		{
			glm::vec3(0.5, 0, 1),
			glm::vec2(0.2, 0.5),
			3,
			0,
			false,
		},
	};
}

SlowmoScene::~SlowmoScene() {
}

void SlowmoScene::on_enter() {
	log_info("Slowmo scene is entered\n");
}

void SlowmoScene::on_exit() {
	log_info("Slowmo scene is exited\n");
}

void SlowmoScene::on_update(f64 dt) {
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

	// Handling dead eye slowmo
	if (m_start_deadeye) {

		// Rendering the time bar
		u32 deadeye_time_elapsed = SDL_GetTicks() - m_deadeye_start_time;
		f32 progress = (100.0f - (((f32) deadeye_time_elapsed / (f32) DEAD_EYE_TIME) * 100.0f)) / 100.0f;

		if (progress <= 0.0f)
			progress = 0.0f;

		rp_push_quad(
			gs.quad_rp,
			glm::vec3(-0.5, -0.5, 0),
			glm::vec2(progress, 0.1),
			glm::vec4(0.5, 0.5, 0, 1),
			gs.quad_rp->white_texture.id,
			glm::vec4(0, 0, 1, 1)
		);

		// Switching to normal tick when dead eye effect is over
		if (deadeye_time_elapsed >= DEAD_EYE_TIME) {
			m_tick = TICK;
		} else {

			// When the deadeye effect is not completed let the player aim

			// TODO: Implement the aiming system

			for (auto& enemy: m_enemies) {
				i32 mp_x, mp_y;
				SDL_GetMouseState(&mp_x, &mp_y);

				// Converting to Normalized Device Coordinates
				float x = (2.0f * mp_x) / WIN_WIDTH - 1.0f;
				float y = 1.0f - (2.0f * mp_y) / WIN_HEIGHT;
				glm::vec4 ray_ndc = glm::vec4(x, y, -1.0f, 1.0f);

				// Converting to View Space
				glm::vec4 ray_clip = glm::vec4(ray_ndc.x, ray_ndc.y, -1.0, 1.0);
				glm::vec4 ray_eye = glm::inverse(gs.camera->proj) * ray_clip;
				ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0); // direction

				// Converting to world space
				glm::vec3 ray_world = glm::normalize(glm::vec3(glm::inverse(gs.camera->look_at) * ray_eye));
				glm::vec3 ray_origin = glm::vec3(glm::inverse(gs.camera->look_at)[3]); // camera position

				// std::cout << ray_origin.x << " " << ray_origin.y << " " << ray_origin.z << std::endl;
				// std::cout << ray_world.x << " " << ray_world.y << " " << ray_world.z << std::endl;
				// std::cout << std::endl;

				// Extracting points from quad
				glm::vec3 v0 = enemy.pos;
				glm::vec3 v1 = { enemy.pos.x + enemy.size.x, enemy.pos.y, enemy.pos.z };
				glm::vec3 v2 = { enemy.pos.x + enemy.size.x, enemy.pos.y + enemy.size.y, enemy.pos.z };
				glm::vec3 v3 = { enemy.pos.x, enemy.pos.y + enemy.size.y, enemy.pos.z };
				glm::vec3 hitPoint;

				// Calculating hit
				bool hit = intersectRayTriangle(ray_origin, ray_world, v0, v1, v2, hitPoint) ||
				           intersectRayTriangle(ray_origin, ray_world, v2, v3, v0, hitPoint);
			}
		}

		// Updating the enemies reaction time
		for (auto& enemy : m_enemies) {
			enemy.curr_time += ENEMY_REACTION_TIME * dt * m_tick;

			if (enemy.react_time <= enemy.curr_time) {
				enemy.reacted = true;
			}
		}
	}

	// Rendering the enemies
	for (auto& enemy : m_enemies) {
		if (enemy.reacted) {
			rp_push_quad(
				gs.quad_rp,
				enemy.pos,
				enemy.size,
				glm::vec4(1, 0, 0, 1),
				gs.quad_rp->white_texture.id,
				glm::vec4(0, 0, 1, 1)
			);
		} else {
			rp_push_quad(
				gs.quad_rp,
				enemy.pos,
				enemy.size,
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

void SlowmoScene::on_event(SDL_Event event, f64 dt) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {

			// Starting the deadeye
			case SDLK_SPACE:
				m_start_deadeye = true;
				m_tick = SLOWMO_TICK;
				m_deadeye_start_time = SDL_GetTicks();
				break;

			// Reset the deadeye
			case SDLK_ESCAPE: {
				m_start_deadeye = false;
				m_tick = TICK;
				m_deadeye_start_time = SDL_GetTicks();

				for (auto& enemy : m_enemies) {
					enemy.curr_time = 0;
					enemy.reacted = false;
				}

			} break;
		}
	}
}

void SlowmoScene::on_imgui_render() {
}
