#include "core.h"

enum AudioID {
	JUMP,
	MUSIC,
};

int main(int argc, char* argv[]) {
	Window window = window_create(640, 480, "Game6 v0.0").unwrap();
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

	// Main loop
	while(running){
		while(SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if(event.type == SDL_QUIT) {
				running = false;
			}

			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
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


		glc(glClearColor(1.0f, 0.9f, 0.1f, 1.0f));
		glc(glClear(GL_COLOR_BUFFER_BIT));

		rp_begin(&rp);
		Vertices v = rp_create_quad(glm::vec3(0, 0, 0), glm::vec2(0.5, 0.5), glm::vec4(1, 0, 0, 1));
		rp_push_vertices(&rp, v);
		rp_end(&rp);

		imgui_begin_frame();
			ImGui::Begin("Hello");
			ImGui::Text("Hello");
			ImGui::End();

			ImGui::Begin("World");
			ImGui::Text("World");
			ImGui::End();
		imgui_end_frame();


		window_swap(window);
	}

	imgui_destroy();
	audio_destroy(audio);
	rp_destroy(&rp);
	window_destroy(&window);
	return 0;
}
