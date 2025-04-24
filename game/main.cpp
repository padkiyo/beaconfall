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

	audio_register_chunk(audio, JUMP, "./jump.wav");
	audio_register_music(audio, MUSIC, "./music.mp3");

	bool running = true;
	SDL_Event event;

	log_info("Opengl Version: %s\n", window_gl_version(window).c_str());

	RenderPipelineSpecs specs = {
		.format = {
			{ .type = GL_FLOAT, .count = 3 },
			{ .type = GL_FLOAT, .count = 4 },
		},
		.max_vertices = 3,
		.shaders = { .vertex_shader = "./game/vertex.vert", .fragment_shader = "./game/fragment.frag"},
	};

	RenderPipeline rp = rp_create(&specs).unwrap();

	Camera camera = camera_create(glm::vec3(0, 0, -2), glm::vec3(0, 0, -1), {
		.aspect_ratio = (f32) WIN_WIDTH / WIN_HEIGHT,
		.fov = 45.0f,
		.near = 0.1f,
		.far = 1000.0f
	});

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
						camera.pos.z += 0.1f;
						break;
					case SDLK_s:
						camera.pos.z -= 0.1f;
						break;
					case SDLK_a:
						camera.pos.x -= 0.1f;
						break;
					case SDLK_d:
						camera.pos.x += 0.1f;
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
		glc(glClearColor(1.0f, 0.9f, 0.1f, 1.0f));
		glc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		rp_begin(&rp);

		glm::mat4 mvp = camera_calc_mvp(camera);
		i32 loc = glc(glGetUniformLocation(rp.shader, "mvp"));
		glc(glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]));

		Vertices v1 = rp_create_quad(glm::vec3(0, -0.7, 1), glm::vec2(0.2, 0.3), glm::vec4(1, 0, 0, 1));
		rp_push_vertices(&rp, v1);

		Vertices v2 = rp_create_quad(glm::vec3(-2, -1, 1), glm::vec2(4, 0.3), glm::vec4(1, 0, 1, 1));
		rp_push_vertices(&rp, v2);

		Vertices v3 = rp_create_quad(glm::vec3(-0.5, -1.2, 2), glm::vec2(1, 2.5), glm::vec4(0, 1, 0, 1));
		rp_push_vertices(&rp, v3);

		Vertices v4 = rp_create_quad(glm::vec3(-1.6, -1.2, 2.5), glm::vec2(1, 2.5), glm::vec4(0, 1, 1, 1));
		rp_push_vertices(&rp, v4);

		Vertices v5 = rp_create_quad(glm::vec3(0.6, -1.2, 2), glm::vec2(1, 2), glm::vec4(0.1, 1, 0.5, 1));
		rp_push_vertices(&rp, v5);

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

	imgui_destroy();
	audio_destroy(audio);
	rp_destroy(&rp);
	window_destroy(&window);
	return 0;
}
