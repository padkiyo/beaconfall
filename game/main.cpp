#include "core.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

enum AudioID {
	JUMP,
	MUSIC,
};

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
			.vertex_shader = "./game/vertex.vert",
			.fragment_shader = "./game/fragment.frag"
		},
	};
	RenderPipeline rp = rp_create(&specs).unwrap();

	// Generating texture samples
	i32 samplers[32];
	for (i32 i = 0; i < 32; i++)
		samplers[i] = i;

	// Providing samplers to the shader
	glc(glUseProgram(rp.shader));
	i32 loc = glc(glGetUniformLocation(rp.shader, "textures"));
	panic(loc != -1, "Cannot find uniform: textures\n");
	glc(glUniform1iv(loc, 32, samplers));

	Camera camera = camera_create(glm::vec3(0, 0, 0), {
		.left = 0,
		.right = WIN_WIDTH,
		.top = 0,
		.bottom = WIN_HEIGHT,
		.near = -1.0f,
		.far = 1000.0f
	});

	Font font = font_create("font.ttf", 32).unwrap();

	audio_register_chunk(audio, JUMP, "./jump.wav");
	audio_register_music(audio, MUSIC, "./music.mp3");

	bool running = true;
	SDL_Event event;

	log_info("Opengl Version: %s\n", window_gl_version(window).c_str());

	// Main loop
	while(running){
		while(SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
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
		glc(glClearColor(0.0f, 0.0f, 1.0f, 1.0f));
		glc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		rp_begin(&rp);

		// Updating camera matrix
		glm::mat4 mvp = camera_calc_mvp(&camera);
		loc = glc(glGetUniformLocation(rp.shader, "mvp"));
		glc(glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]));

		rp_push_quad(&rp, glm::vec3(0, 0, 0), glm::vec2(50, 50), glm::vec4(1, 0, 0, 1), rp.white_texture.id, glm::vec4(0, 0, 1, 1));
		rp_push_text(&rp, &font, "Hello world", glm::vec3(100, 100, 0), glm::vec4(1, 1, 1, 1));

		rp_end(&rp);

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

	font_destroy(&font);
	imgui_destroy();
	audio_destroy(audio);
	rp_destroy(&rp);
	window_destroy(&window);
	return 0;
}
