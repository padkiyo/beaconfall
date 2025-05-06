#include "core.h"
#include "config.h"
#include "game_state.h"

#include "systems/map_system/map_system.h"
#include "systems/map_system/maps.h"
#include "systems/dialog_system/dialog_system.h"
#include "systems/dialog_system/dialogs.h"
#include "systems/notebook_system/notebook_system.h"
#include "scenes/scenes.h"

// Global game state
GameState gs;

int main(int argc, char* argv[]) {

	// Initialization of window
	Window window = window_create(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE).unwrap();

	// Setting up the imgui
	imgui_create(window.sdl_window, window.gl_context);

	// Creating audio manager
	Audio* audio = audio_create().unwrap();

	// Creating the default quad renderer
	RenderPipeline quad_rp = rp_create(&QuadRendererSpecs).unwrap();
	init_texture_samples(&quad_rp);

	// Creating an orthographic camera
	Camera camera = camera_create(glm::vec3(0, 0, 0), {
		.left = 0,
		.right = WIN_WIDTH,
		.top = 0,
		.bottom = WIN_HEIGHT,
		.near = -1.0f,
		.far = 1000.0f
	});

	// Dialog system
	DialogSystem* ds = dialog_system_create();

	// Loading up the dialogs
	dialog_system_init_dialogs(ds);

	// Notebook system
	NotebookSystem ns = notebook_system_create();

	// Creating scene manager
	SceneManager sm = sm_create();

	// Creating Map Manager
	MapManager mm = mm_create();

	// Adding the scenes
	sm_add_scene(
		&sm, SCENE_DIALOG,
		dialog_entry,
		dialog_exit,
		dialog_update,
		dialog_event,
		NULL
	);

	sm_add_scene(
		&sm, SCENE_MAP,
		map_entry,
		map_exit,
		map_update,
		map_event,
		NULL
	);

	sm_add_scene(
		&sm, SCENE_NOTEBOOK,
		notebook_entry,
		notebook_exit,
		notebook_update,
		notebook_event,
		NULL
	);

	// Switching the scene
	sm_switch_scene(&sm, SCENE_MAP);

	// Adding map to map manager
	mm_add_map(&mm, TEST_MAP, "./assets/maps/test_map/map.json", "./assets/maps/test_map/spritesheet.png").unwrap();

	mm_add_map(&mm, TEST2_MAP, "./assets/maps/test2_map/map.json", "./assets/maps/test2_map/spritesheet.png").unwrap();
	// Loading fonts
	Font font_regular = font_create("./assets/Ac437_ToshibaSat_9x8.ttf", 25).unwrap();

	// Initializing the game state
	gs.window = &window;
	gs.quad_rp = &quad_rp;
	gs.audio = audio;
	gs.camera = &camera;
	gs.sm = &sm;
	gs.ds = ds;
	gs.ns = &ns;
	gs.mm = &mm;
	gs.font_regular = &font_regular;
	log_info("Opengl Version: %s\n", window_gl_version(window).c_str());

	// Main loop
	bool running = true;
	while(running) {

		// Event loop
		SDL_Event event;
		while(SDL_PollEvent(&event)) {

			// Events for imgui
			ImGui_ImplSDL2_ProcessEvent(&event);

			// TODO: Maybe maintain some state on which even is running?
			// Handling event according to the running system
			if (ds->is_dialog_running) {
				dialog_system_handle_event(ds, event);
			} else if (ns.open) {
				notebook_system_handle_event(&ns, event);
			} else {
				// Events for scenes
				sm_handle_event(&sm, event, 0);
			}

			if(event.type == SDL_QUIT) {
				running = false;
			}

			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				}
			}
		}

		// Resource bindings
		font_bind(&font_regular);
		glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

		glc(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
		glc(glClear(GL_COLOR_BUFFER_BIT));

		// Scene rendering section
		rp_begin(&quad_rp);
		{
			// Updating camera matrix
			glm::mat4 mvp = camera_calc_mvp(&camera);
			i32 loc = glc(glGetUniformLocation(quad_rp.shader, "mvp"));
			glc(glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]));

			// Updating the current scene
			sm_update_scene(&sm, 0);

			// Updating dialog system
			dialog_system_update(ds);

			// Updating notebook system
			notebook_system_update(&ns);
		}
		rp_end(&quad_rp);

		imgui_begin_frame();

		if(ImGui::CollapsingHeader("Scenes"))
		{
			ImGui::SeparatorText("Scene Manager");
			for(auto & [key, value] : sm.scenes){
				std::string button_name = sm.current_scene != key? "[ ] " + scene_name(key) : "[*] " + scene_name(key);
				if(ImGui::Button(button_name.c_str()))
				{
					sm_switch_scene(&sm, key);
				}
			}
		}

		if(ImGui::CollapsingHeader("Notebook"))
		{
			std::string text;
			if (ImGui::InputText("text", &text, ImGuiInputTextFlags_EnterReturnsTrue)) {
				notebook_system_append_text(&ns, text);
			}
		}

		if(ImGui::CollapsingHeader("Map Manager"))
		{
			ImGui::SeparatorText("Map Manager");
			for(auto & [key, value] : mm.maps){
				std::string button_name = mm.current_map != key? "[ ] " + map_name(key) : "[*] " + map_name(key);
				if(ImGui::Button(button_name.c_str()))
				{
					mm_switch_map(&mm, key);
				}
			}

		}

		imgui_end_frame();

		window_swap(window);
	}

	font_destroy(&font_regular);

	dialog_system_destroy(ds);
	imgui_destroy();
	audio_destroy(audio);
	rp_destroy(&quad_rp);
	window_destroy(&window);
	return 0;
}
