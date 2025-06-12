#include "game.h"
#include "config.h"
#include "scenes/scenes.h"
#include "systems/map_system/map_system.h"
#include "systems/map_system/maps.h"

Game::Game() {
	init_core();
	init_scenes();
	init_systems();
	init_resources();

	m_running = true;
}

Game::~Game() {
	// Freeing resources
	font_destroy(m_gs.font_regular);

	// Destroying the systems
	mm_destroy(m_gs.mm);

	// Core deinitializatioin
	rp_destroy(m_gs.quad_rp);

	delete m_gs.camera;
	delete m_gs.fc;
	delete m_gs.scene_mgr;
	delete m_gs.audio_mgr;
	delete m_gs.window;
}


/*
 * This is the main loop of the game
 * Everything of the game system goes here
 */

void Game::run() {
	while (m_running) {
		m_gs.fc->begin();

		// Binds the game resources
		use_resources();

		event();
		render();
		overlay();
		imgui_render();

		m_gs.window->swap();
		m_gs.fc->end();
	}
}


/*
 * This is the event handler of the game
 * It polls and distributes the events to scenes and systems
 * Anything related to events goes here.
 */

void Game::event() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {

		// Events for imgui
		ImGui_ImplSDL2_ProcessEvent(&event);

		// Event for the scene
		m_gs.scene_mgr->handle_event(event, m_gs.fc->dt());

		if(event.type == SDL_QUIT) {
			m_running = false;
		}

		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_q:
					m_gs.camera->change_pos({0,0,1}, CAM_SPEED);
					break;
				case SDLK_e:
					m_gs.camera->change_pos({0,0,-1}, CAM_SPEED);
					break;

				case SDLK_w:
					m_gs.camera->change_pos({0,1,0}, CAM_SPEED);
					break;
				case SDLK_s:
					m_gs.camera->change_pos({0,-1,0}, CAM_SPEED);
					break;

				case SDLK_a:
					m_gs.camera->change_pos({-1,0,0}, CAM_SPEED);
					break;
				case SDLK_d:
					m_gs.camera->change_pos({1,0,0}, CAM_SPEED);
					break;
			}
		}
	}
}


/*
 * This is where actual rendering is done.
 * Things that needs to be rendered goes in here.
 */

void Game::render() {
	GLC(glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT));
	GLC(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
	GLC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// Updating camera matrix
	GLC(glUseProgram(m_gs.quad_rp->shader));
	m_gs.camera->recalculate_view_proj();
	glm::mat4 mvp = m_gs.camera->get_view_proj();
	i32 loc = GLC(glGetUniformLocation(m_gs.quad_rp->shader, "mvp"));
	GLC(glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]));

	// Scene rendering section
	rp_begin(m_gs.quad_rp);
	{
		// Updating the current scene
		m_gs.scene_mgr->update_current_scene(m_gs.fc->dt());
	}
	rp_end(m_gs.quad_rp);
}


/*
 * This is the overlay layer of the game.
 * This draws the stuff ontop of the game.
 * So any UI stuff that needs to stay ontop of the game regardless of scene goes here
 */

void Game::overlay() {
	rp_begin(m_gs.quad_rp);
	{
		// Rendering the FPS counter
		std::string fps = std::to_string(m_gs.fc->fps());
		rp_push_text(
			m_gs.quad_rp,
			m_gs.font_regular,
			fps,
			glm::vec3(0, 0, 1),
			glm::vec4(0, 1, 0.2, 1)
		);
	}
	rp_end(m_gs.quad_rp);
}


/*
 * This is where ImGui frames are gonna get rendered
 * If any imgui related frames needs to be rendered regardless of scene,
 * then add here.
 */

void Game::imgui_render() {
	imgui_begin_frame();

	// Updating imgui render of the scenes
	m_gs.scene_mgr->update_imgui_render();

	if(ImGui::CollapsingHeader("Scenes")) {
		ImGui::SeparatorText("Scene Manager");
		for(auto& [key, value] : m_gs.scene_mgr->get_scenes()) {
			std::string button_name =
				m_gs.scene_mgr->get_current_scene() != key
				? "[ ] " + scene_name(key)
				: "[*] " + scene_name(key);
			if(ImGui::Button(button_name.c_str())) {
				m_gs.scene_mgr->switch_scene(key);
			}
		}
	}

	if(ImGui::CollapsingHeader("Map Manager")) {
		ImGui::SeparatorText("Map Manager");
		for(auto & [key, value] : m_gs.mm->maps) {
			std::string button_name =
				m_gs.mm->current_map != key
				? "[ ] " + map_name(key)
				: "[*] " + map_name(key);
			if(ImGui::Button(button_name.c_str())) {
				mm_switch_map(m_gs.mm, key);
			}
		}
	}

	imgui_end_frame();
}


/*
 * Here we initialize our Core Engine
 * Anything regardless of core needs to be initialized here
 */

void Game::init_core() {

	// Intializing the window
	m_gs.window = new Window(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);

	// Initializing the imgui context
	imgui_create(m_gs.window->get_sdl_window(), m_gs.window->get_gl_context());

	// Initializing audio manager
	m_gs.audio_mgr = new AudioManager;

	// Initializing scene manager
	m_gs.scene_mgr = new SceneManager;

	// Initializing the render pipeline
	m_gs.quad_rp = rp_create(&QuadRendererSpecs).unwrap();
	init_texture_samples(m_gs.quad_rp);

	// Initializing orthographic camera
	m_gs.camera = new Camera(glm::vec3(0, 0, -2), glm::vec3(0, 0, -1), {
		.fov = 45.0f,
		.aspect_ratio = WIN_WIDTH/WIN_HEIGHT,
		.near = 0.1f,
		.far = 1000.0f,
	});

	// Initializing the frame controller
	m_gs.fc = new FrameController(FPS);
}


/*
 * This is the place where we actually append our scenes of the game
 */

void Game::init_scenes() {
	m_gs.scene_mgr->add_scene<MapScene>(SCENE_MAP, m_gs);
	m_gs.scene_mgr->add_scene<SlowmoScene>(SCENE_SLOWMO, m_gs);

	m_gs.scene_mgr->switch_scene(SCENE_SLOWMO);
}


/*
 * Here we initialize our systems that are in the game
 */

void Game::init_systems() {
	// Initializing map manager
	m_gs.mm = mm_create();

	// Adding maps
	mm_add_map(
		m_gs.mm, TEST_MAP,
		"./assets/maps/test_map/map.json",
		"./assets/maps/test_map/spritesheet.png"
	).unwrap();

	mm_add_map(
		m_gs.mm, TEST2_MAP,
		"./assets/maps/test2_map/map.json",
		"./assets/maps/test2_map/spritesheet.png"
	).unwrap();
}


/*
 * Any resources that game requires are initialized here
 */

void Game::init_resources() {
	m_gs.font_regular = font_create("./assets/Ac437_ToshibaSat_9x8.ttf", 25).unwrap();
}


/*
 * This function binds the resources that the game gonna use
 */

void Game::use_resources() {
	font_bind(m_gs.font_regular);
}
