#include "game.h"
#include "config.h"
#include "scenes/scenes.h"
// #include "systems/map_system/map_system.h"
// #include "systems/map_system/maps.h"
//

Game::Game() {
	init_core();
	init_scenes();
	init_systems();
	init_resources();

	l1 = {
		.pos = {130, 197},
		.radius = 0.5f,
		.intensity = 0.45f,
		.dir = glm::radians(81.0f) ,
		.fov = glm::radians(41.0f),
		.color = {0.8,0.5,0,1}
	};

	l2 = {
		.pos = {347, 225},
		.radius = 0.5f,
		.intensity = 0.610f,
		.dir = glm::radians(180.0f),
		.fov = glm::radians(360.0f),
		.color = {1.0f,0.522,0.04,1}
	};

	a = Rect({400, 350, 50, 50});
	boxes = {
		Rect({10, 10, 200, 20}),
		Rect({10, 20, 20, 200}),
		Rect({200, 10, 200, 20})
	};

	m_running = true;

	TextureFilter sprite_filter = {
				.min_filter = GL_LINEAR,
				.mag_filter = GL_LINEAR,
				.wrap_s = GL_CLAMP_TO_EDGE,
				.wrap_t = GL_CLAMP_TO_EDGE,
				.flip = true
			};
}

Game::~Game() {
	// Freeing resources
	font_destroy(m_gs.font_regular);

	// Core deinitializatioin
	delete m_gs.camera;
	delete m_gs.fc;
	delete m_gs.scene_mgr;
	delete m_gs.audio_mgr;
	delete m_gs.renderer;
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
					move.y += move_speed;
					// m_gs.camera->change_pos({0,1,0}, CAM_SPEED);
					break;
				case SDLK_s:
					move.y -= move_speed;
					// m_gs.camera->change_pos({0,-1,0}, CAM_SPEED);
					break;

				case SDLK_a:
					move.x -= move_speed;
					// m_gs.camera->change_pos({-1,0,0}, CAM_SPEED);
					break;
				case SDLK_d:
					move.x += move_speed;
					// m_gs.camera->change_pos({1,0,0}, CAM_SPEED);
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
	m_gs.renderer->set_clear_color({0.5,0.5,0.5,1});
	m_gs.renderer->set_ambient_color(ambient_color);
	m_gs.renderer->set_light_pixel_size({pixel_size, pixel_size});

	m_gs.renderer->begin(*m_gs.camera);

	m_gs.renderer->push_light(l1);
	m_gs.renderer->push_light(l2);

	m_gs.map->render();

	m_gs.sprt_mgr->activate_spritesheet(PLAYER);
	m_gs.renderer->push_quad(
		{a.x, a.y, 0},
		{a.w, a.h},
		glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), {0,0,1}),
		m_gs.sprt_mgr->get_spritesheet_texture(PLAYER),
		m_gs.anim_mgr->get_frame(*m_gs.sprt_mgr),
		{1,1,1,1}
	);



	for (auto box : boxes) {
		m_gs.renderer->push_quad(
			{box.x, box.y, 0},
			{box.w, box.h},
			glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), {0,0,1}),
			m_gs.renderer->white_texture(),
			{0,0,1,1},
			{1,0,1,1}
		);
	}


	a.resolve(boxes, move, m_gs.fc->dt());

	std::cout
		<< "left: " << a.intersect_at_left() << "\n"
		<< "right: " << a.intersect_at_right() << "\n"
		<< "up: " << a.intersect_at_up() << "\n"
		<< "down: " << a.intersect_at_down() << "\n"
		<< std::endl;


	m_gs.renderer->end();

	// Reset move
	move = glm::vec2(0);
}


/*
 * This is the overlay layer of the game.
 * This draws the stuff ontop of the game.
 * So any UI stuff that needs to stay ontop of the game regardless of scene goes here
 */

void Game::overlay() {
	// rp_begin(m_gs.quad_rp);
	// {
	// 	// Rendering the FPS counter
	// 	std::string fps = std::to_string(m_gs.fc->fps());
	// 	rp_push_text(
	// 		m_gs.quad_rp,
	// 		m_gs.font_regular,
	// 		fps,
	// 		glm::vec3(0, 0, 1),
	// 		glm::vec4(0, 1, 0.2, 1)
	// 	);
	// }
	// rp_end(m_gs.quad_rp);
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

	if (ImGui::CollapsingHeader("Setting")) {
		ImGui::DragFloat("Speed", &move_speed, 10.0f, 1.0f, 1000.0f);
	}

	if (ImGui::CollapsingHeader("Light")) {
		ImGui::ColorEdit3("Ambient Color", (f32*) &ambient_color);
		ImGui::DragFloat("Pixel Size", &pixel_size, 0.1f, 1.0f, 50.0f);

		ImGui::SeparatorText("Light 1");
		ImGui::DragFloat2("Position1", (float*)&l1.pos, 1.0f, -500.0f, 500.0f);
		ImGui::DragFloat("Radius1", &l1.radius, 0.5f, 0.0f, 50.0f);
		ImGui::DragFloat("Intensity1", &l1.intensity, 0.01f, 0.0f, 10.0f);
		ImGui::SliderAngle("Direction1", &l1.dir, 0.0f, 360.0f);
		ImGui::SliderAngle("FOV1", &l1.fov, 0.0f, 360.0f);
		ImGui::ColorEdit4("Color1", (float*)&l1.color);

		ImGui::SeparatorText("Light 2");
		ImGui::DragFloat2("Position2", (float*)&l2.pos, 1.0f, -500.0f, 500.0f);
		ImGui::DragFloat("Radius2", &l2.radius, 0.5f, 0.0f, 50.0f);
		ImGui::DragFloat("Intensity2", &l2.intensity, 0.01f, 0.0f, 10.0f);
		ImGui::SliderAngle("Direction2", &l2.dir, 0.0f, 360.0f);
		ImGui::SliderAngle("FOV2", &l2.fov, 0.0f, 360.0f);
		ImGui::ColorEdit4("Color2", (float*)&l2.color);
	}

	if(ImGui::CollapsingHeader("Animator")){
		ImGui::SeparatorText("Player");
		if (ImGui::Button("Idle")) {
					m_gs.anim_mgr->switch_frame(PLAYER_IDLE);
		}

		if(ImGui::Button("Die")) {
					m_gs.anim_mgr->switch_frame(PLAYER_DIE);

		}
	}

	/*
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
	*/

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
	//m_gs.quad_rp = rp_create(&QuadRendererSpecs).unwrap();
	//init_texture_samples(m_gs.quad_rp);
	m_gs.renderer = new Renderer({WIN_WIDTH, WIN_HEIGHT});

	// Initializing perspective camera
	// m_gs.camera = new Camera(glm::vec3(0, 0, -2), glm::vec3(0, 0, -1), {
	// 	.fov = 45.0f,
	// 	.aspect_ratio = WIN_WIDTH/WIN_HEIGHT,
	// 	.near = 0.1f,
	// 	.far = 1000.0f,
	// });
	m_gs.camera = new Camera(glm::vec3(0,0,0), {
		.left = 0,
		.right = WIN_WIDTH,
		.top = WIN_HEIGHT,
		.bottom = 0,
		.near = 0,
		.far = 1000,
	});

	// Initializing the frame controller
	m_gs.fc = new FrameController(FPS);
}


/*
 * This is the place where we actually append our scenes of the game
 */

void Game::init_scenes() {
	// m_gs.scene_mgr->add_scene<MapScene>(SCENE_MAP, m_gs);
	// m_gs.scene_mgr->add_scene<SlowmoScene>(SCENE_SLOWMO, m_gs);
	m_gs.scene_mgr->add_scene<TestScene>(SCENE_TEST);

	m_gs.scene_mgr->switch_scene(SCENE_TEST);
}


/*
 * Here we initialize our systems that are in the game
 */

void Game::init_systems() {
	/*
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
	*/
	m_gs.sprt_mgr = new SpriteManager();

	auto sprite_id = PLAYER;
	auto init_anim = PLAYER_IDLE;

	m_gs.anim_mgr = new Animator(PLAYER, PLAYER_IDLE);
}


/*
 * Any resources that game requires are initialized here
 */

void Game::init_resources() {
	m_gs.font_regular = font_create("./assets/Ac437_ToshibaSat_9x8.ttf", 25).unwrap();

	// loading sprites
	Sprite player_sprite = {
		.path = "./assets/samurai.png",
		.x_cnt = 14, // No of horizontal sprites
		.y_cnt = 8 // No of vertical sprites
	};

	m_gs.sprt_mgr->add_sprite(player_sprite, PLAYER);
	m_gs.sprt_mgr->create_frame(PLAYER, 7,7, PLAYER_IDLE);
	m_gs.sprt_mgr->create_frame(PLAYER, 0, 13, PLAYER_DIE);
	m_gs.anim_mgr->add_animation(PLAYER_IDLE, 100 * 8, true);
	m_gs.anim_mgr->add_animation(PLAYER_DIE, 100 * 14, false);

	MapEntry map_config= {
		.map_file = "assets/maps/EXAMPLE/map.json",
		.map_tileset = "assets/maps/EXAMPLE/spritesheet.png",
		.renderer = m_gs.renderer,
		.render_scale = 1.50f
	};

	m_gs.map = new Map(map_config);
}


/*
 * This function binds the resources that the game gonna use
 */

void Game::use_resources() {
	font_bind(m_gs.font_regular);
}
