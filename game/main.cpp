#include "core.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

enum AudioID {
	JUMP,
	MUSIC,
};

enum Scenes {
	SCENE_A,
	SCENE_B,
};

// Scene A
void a_enter(void* data) {
	log_info("Entered scene A\n");
}

void a_exit(void* data) {
	log_info("Exited scene A\n");
}

void a_update(void* data, f64 dt) {
	log_info("Updating scene A\n");
}

void a_event(void* data, SDL_Event event, f64 dt) {
	log_info("Event in scene A\n");
}

// Scene B
void b_enter(void* data) {
	log_info("Entered scene B\n");
}

void b_exit(void* data) {
	log_info("Exited scene B\n");
}

void b_update(void* data, f64 dt) {
	log_info("Updating scene B\n");
}

void b_event(void* data, SDL_Event event, f64 dt) {
	log_info("Event in scene B\n");
}

int main(int argc, char* argv[]) {
	Window window = window_create(WIN_WIDTH, WIN_HEIGHT, "Game6 v0.0").unwrap();
	Audio* audio = audio_create().unwrap();
	imgui_create(window.sdl_window, window.gl_context);

	RenderPipelineSpecs specs = {
		.format = {
			{ .type = GL_FLOAT, .count = 3 },
			{ .type = GL_FLOAT, .count = 4 },
			{ .type = GL_FLOAT, .count = 2 },
			{ .type = GL_FLOAT, .count = 1 },
		},
		.max_vertices = 10000,
		.shaders = {
			.vertex_shader = "./shaders/vertex.vert",
			.fragment_shader = "./shaders/fragment.frag"
		},
	};
	RenderPipeline rp = rp_create(&specs).unwrap();

	RenderPipelineSpecs frame_specs = {
		.format = {
			{ .type = GL_FLOAT, .count = 3 },
			{ .type = GL_FLOAT, .count = 2 },
		},
		.max_vertices = 1000,
		.shaders = {
			.vertex_shader = "./shaders/frame.vert",
			.fragment_shader = "./shaders/frame.frag"
		},
	};

	RenderPipeline frame_rp = rp_create(&frame_specs).unwrap();

	FrameBuffer fb = fb_create(WIN_WIDTH, WIN_HEIGHT);

	// Generating texture samples
	i32 samplers[32];
	for (i32 i = 0; i < 32; i++)
		samplers[i] = i;

	// Providing samplers to the shader
	glc(glUseProgram(rp.shader));
	i32 loc = glc(glGetUniformLocation(rp.shader, "textures"));
	panic(loc != -1, "Cannot find uniform: textures\n");
	glc(glUniform1iv(loc, 32, samplers));

	// Frame buffer shader
	glc(glUseProgram(frame_rp.shader));
	loc = glc(glGetUniformLocation(frame_rp.shader, "textures"));
	panic(loc != -1, "Cannto find uniform: texture\n");
	glc(glUniform1i(loc, fb.color_texture.id));

	// Code for black and white trigger
	i32 bnw_loc = glc(glGetUniformLocation(frame_rp.shader, "bnw"));
	panic(loc != -1, "Cannot find unfirom: bnw");
	float bnw = 0.0f;

	Camera camera = camera_create(glm::vec3(0, 0, 0), {
		.left = 0,
		.right = WIN_WIDTH,
		.top = 0,
		.bottom = WIN_HEIGHT,
		.near = -1.0f,
		.far = 1000.0f
	});

	// Scene
	SceneManager sm = {0};
	sm_add_scene(&sm, SCENE_A, a_enter, a_exit, a_update, a_event, NULL);
	sm_add_scene(&sm, SCENE_B, b_enter, b_exit, b_update, b_event, NULL);

	Font small_font = font_create("font.ttf", 32).unwrap();
	Font big_font = font_create("font.ttf", 100).unwrap();

	audio_register_chunk(audio, JUMP, "./jump.wav");
	audio_register_music(audio, MUSIC, "./music.mp3");

	bool running = true;
	SDL_Event event;

	log_info("Opengl Version: %s\n", window_gl_version(window).c_str());

	// Main loop
	while(running){
		while(SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);

			sm_handle_event(&sm, event, 0);

			if(event.type == SDL_QUIT) {
				running = false;
			}

			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_w:
						camera.pos.y -= 10.1f;
						break;
					case SDLK_s:
						camera.pos.y += 10.1f;
						break;
					case SDLK_a:
						camera.pos.x -= 10.1f;
						break;
					case SDLK_d:
						camera.pos.x += 10.1f;
						break;
					case SDLK_j:
						audio_play_chunk(audio, JUMP);
						break;
					case SDLK_b:
						if (bnw == 0.0f)
						{
							bnw = 1.0f;
						}
						else{
							bnw = 0.0f;
						}
						break;

					case SDLK_TAB:
						if (sm.current_scene == SCENE_A) {
							sm_switch_scene(&sm, SCENE_B);
						} else {
							sm_switch_scene(&sm, SCENE_A);
						}
						break;

					case SDLK_SPACE:
						if (!audio_is_music_playing()) {
							audio_play_music(audio, MUSIC);
						} else {
							if (audio_paused_music()) {
								audio_resume_music();
							} else {
								audio_pause_music();
							}
						}
						break;
				}
			}
		}

		glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

		// Binding Frame Buffer
		fb_bind(&fb);

		glc(glClearColor(1.0f, 0.0f, 1.0f, 1.0f));
		glc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		glc(glEnable(GL_DEPTH_TEST));

		rp_begin(&rp);

		sm_update_scene(&sm, 0);

		// Dont forget to bind those fonts
		font_bind(&small_font);
		font_bind(&big_font);

		// Updating camera matrix
		glm::mat4 mvp = camera_calc_mvp(&camera);
		loc = glc(glGetUniformLocation(rp.shader, "mvp"));
		glc(glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]));

		rp_push_quad(&rp, glm::vec3(0, 0, 0), glm::vec2(50, 50), glm::vec4(1, 0, 0, 1), rp.white_texture.id, glm::vec4(0, 0, 1, 1));

		rp_push_text(&rp, &small_font, "Hello world", glm::vec3(100, 100, 0), glm::vec4(1, 1, 1, 1));
		rp_push_text(&rp, &big_font, "Hello world", glm::vec3(100, 200, 0), glm::vec4(1, 1, 1, 1));

		rp_end(&rp);

		rp_begin(&frame_rp);

		// UNBINDING FRAME BUFFEr
		fb_unbind(&fb);

		glc(glClearColor(1.0f, 0.0f, 1.0f, 1.0f));
		glc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		rp_push_vertices(&frame_rp,  {
				-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,  // Top-left
				-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // Bottom-left
				1.0f,  1.0f, 0.0f,  1.0f, 1.0f,  // Top-right

				1.0f,  1.0f, 0.0f,  1.0f, 1.0f,  // Top-right
				-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // Bottom-left
				1.0f, -1.0f, 0.0f,  1.0f, 0.0f   // Bottom-right
				}
		);

		glc(glUniform1f(bnw_loc, bnw));
		rp_end(&frame_rp);

		imgui_begin_frame();
		// ImGui::Begin("Hello");
		// ImGui::Text("Hello");
		// ImGui::End();

		// ImGui::Begin("World");
		// ImGui::Text("World");
		// ImGui::End();
		imgui_end_frame();


		window_swap(window);
	}

	font_destroy(&small_font);
	font_destroy(&big_font);

	imgui_destroy();
	audio_destroy(audio);
	rp_destroy(&rp);
	window_destroy(&window);
	return 0;
}
