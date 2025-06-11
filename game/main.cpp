#include "core.h"
#include "config.h"
#include "game_state.h"
#include "scenes/scenes.h"
#include "systems/map_system/maps.h"

#define CAM_SPEED 1.0f

extern GameState gs;

int main(int argc, char* argv[]) {

	// Initializations of game state
	gs_init_core();
	gs_init_scenes();
	gs_init_systems();
	gs_init_resources();

	log_info("Opengl Version: %s\n", gs.window->get_gl_version().c_str());

	// Main loop
	bool running = true;
	while(running) {

		// Starting the frame count
		fc_start(&gs.fc);

		// Event loop
		SDL_Event event;
		while(SDL_PollEvent(&event)) {

			// Events for imgui
			ImGui_ImplSDL2_ProcessEvent(&event);

			// TODO: Maybe maintain some state on which even is running?
			// Handling event according to the running system
			if (gs.ds->is_dialog_running) {
				dialog_system_handle_event(gs.ds, event);
			} else if (gs.ns->open) {
				notebook_system_handle_event(gs.ns, event);
			} else {
				// Events for scenes
				sm_handle_event(gs.sm, event, gs.fc.dt);
			}

			if(event.type == SDL_QUIT) {
				running = false;
			}

			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_q:
						gs.camera->pos.z += CAM_SPEED;
						break;
					case SDLK_e:
						gs.camera->pos.z -= CAM_SPEED;
						break;

					case SDLK_w:
						gs.camera->pos.y += CAM_SPEED;
						break;
					case SDLK_s:
						gs.camera->pos.y -= CAM_SPEED;
						break;

					case SDLK_a:
						gs.camera->pos.x -= CAM_SPEED;
						break;
					case SDLK_d:
						gs.camera->pos.x += CAM_SPEED;
						break;
				}
			}
		}

		// Resource bindings
		gs_use_resources();

		glc(glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT));
		glc(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
		glc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// Updating camera matrix
		glc(glUseProgram(gs.quad_rp->shader));
		glm::mat4 mvp = camera_calc_mvp(gs.camera);
		i32 loc = glc(glGetUniformLocation(gs.quad_rp->shader, "mvp"));
		glc(glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]));

		// Scene rendering section
		rp_begin(gs.quad_rp);
		{
			// Updating the current scene
			sm_update_scene(gs.sm, gs.fc.dt);

			// Updating dialog system
			dialog_system_update(gs.ds);

			// Updating notebook system
			notebook_system_update(gs.ns);
		}

		std::cout << gs.camera->pos.x << "," << gs.camera->pos.y << "," << gs.camera->pos.z << std::endl;

		rp_end(gs.quad_rp);

		// Overlay rendering
		rp_begin(gs.quad_rp);
		{
			std::string fps = std::to_string(gs.fc.fps);
			rp_push_text(
				gs.quad_rp,
				gs.font_regular,
				fps,
				glm::vec3(0, 0, 1),
				glm::vec4(0, 1, 0.2, 1)
			);
		}
		rp_end(gs.quad_rp);

		imgui_begin_frame();

		if(ImGui::CollapsingHeader("Scenes"))
		{
			ImGui::SeparatorText("Scene Manager");
			for(auto & [key, value] : gs.sm->scenes){
				std::string button_name = gs.sm->current_scene != key? "[ ] " + scene_name(key) : "[*] " + scene_name(key);
				if(ImGui::Button(button_name.c_str()))
				{
					sm_switch_scene(gs.sm, key);
				}
			}
		}

		if(ImGui::CollapsingHeader("Notebook"))
		{
			std::string text;
			if (ImGui::InputText("text", &text, ImGuiInputTextFlags_EnterReturnsTrue)) {
				notebook_system_append_text(gs.ns, text);
			}
		}

		if(ImGui::CollapsingHeader("Map Manager"))
		{
			ImGui::SeparatorText("Map Manager");
			for(auto & [key, value] : gs.mm->maps){
				std::string button_name = gs.mm->current_map != key? "[ ] " + map_name(key) : "[*] " + map_name(key);
				if(ImGui::Button(button_name.c_str()))
				{
					mm_switch_map(gs.mm, key);
				}
			}

		}

		imgui_end_frame();

		gs.window->swap();

		// Capping the frame
		fc_end(&gs.fc);
		// log_info("FPS: %d\n", gs.fc.fps);
		// log_info("dt: %f\n", gs.fc.dt);
	}

	// Cleaning up the game
	gs_free();
	return 0;
}
