#include "game.h"
#include "config.h"
#include "scenes/scenes.h"

Game::Game() {
	init_core();
	init_scenes();
	init_systems();
	init_resources();

	// Init game
	m_running = true;

	// Switching to the scene
	m_gs.scene_mgr->switch_scene(SCENE_GAME);
}

Game::~Game() {
	// Freeing resources
	delete m_gs.font_regular;

	// Core deinitializatioin
	delete m_gs.camera;
	delete m_gs.fc;
	delete m_gs.scene_mgr;
	delete m_gs.audio_mgr;
	delete m_gs.ui;
	delete m_gs.scene_renderer;
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

		// Event for the ui
		m_gs.ui->handle_event(event);

		// Event for the scene
		m_gs.scene_mgr->handle_event(event, m_gs.fc->dt());

		if(event.type == SDL_QUIT) {
			m_running = false;
		}
	}
}


/*
 * This is where actual rendering is done.
 * Things that needs to be rendered goes in here.
 */

void Game::render() {
	// Updating the current scene
	m_gs.scene_mgr->update_current_scene(m_gs.fc->dt());

	// Activating all the sprites
	m_gs.sprt_mgr->activate_spritesheet(PLAYER);
	m_gs.sprt_mgr->activate_spritesheet(ROCK);

	m_gs.renderer->clear({0,0,0,1});

	// Rendering
	m_gs.renderer->start();
	{
		// Rendering the scene
		const Scene& scene = m_gs.scene_mgr->get_current_scene();
		m_gs.scene_renderer->render_scene(scene, *m_gs.camera, { WIN_WIDTH, WIN_HEIGHT });

		// UI rendering
		m_gs.ui->begin();
			m_gs.scene_mgr->render_current_scene_ui(*m_gs.ui);
			overlay();
		m_gs.ui->end();
	}
	m_gs.renderer->commit({ WIN_WIDTH, WIN_HEIGHT });
}


/*
 * This is the overlay layer of the game.
 * This draws the stuff ontop of the game.
 * So any UI stuff that needs to stay ontop of the game regardless of scene goes here
 */

void Game::overlay() {
	std::string fps = std::to_string(m_gs.fc->fps());
	m_gs.ui->text(fps, { 0, 0 }, Style {
		.fg_color = { 0, 1, 0, 1 },
		.font = m_gs.font_regular,
	});
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

	if (ImGui::CollapsingHeader("Stats")) {
		Renderer::Stat stat = m_gs.renderer->get_stat();
		ImGui::Text("Draw Calls: %d", stat.draw_calls);
		ImGui::Text("Vertex Counts: %d", stat.vertex_counts);
	}

	if (ImGui::CollapsingHeader("Scenes")) {
		ImGui::SeparatorText("Scene Manager");
		for (auto& [key, value] : m_gs.scene_mgr->get_scenes()) {
			std::string button_name =
				m_gs.scene_mgr->get_current_scene_id() != key
				? "[ ] " + scene_name(key)
				: "[*] " + scene_name(key);
			if(ImGui::Button(button_name.c_str())) {
				m_gs.scene_mgr->switch_scene(key);
			}
		}
	}

	if (ImGui::CollapsingHeader("Animator")) {
		ImGui::SeparatorText("Player");
		if (ImGui::Button("Idle")) {
			m_gs.anim_mgr->switch_frame(PLAYER_IDLE);
		}

		if (ImGui::Button("Die")) {
			m_gs.anim_mgr->switch_frame(PLAYER_DIE);
		}
	}
	imgui_end_frame();
}


/*
 * Here we initialize our Core Engine
 * Anything regardless of core needs to be initialized here
 */

void Game::init_core() {
	m_gs.window = new Window(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
	m_gs.fc = new FrameController(FPS);
	imgui_create(m_gs.window->get_sdl_window(), m_gs.window->get_gl_context());
	m_gs.audio_mgr = new AudioManager;
	m_gs.scene_mgr = new SceneManager;
	m_gs.renderer = new Renderer;
	m_gs.scene_renderer = new SceneRenderer(m_gs.renderer);
	m_gs.camera = new Camera(glm::vec3(0,0,0), {
		.left = 0,
		.right = WIN_WIDTH,
		.top = WIN_HEIGHT,
		.bottom = 0,
		.near = 0,
		.far = 1000,
	});
	m_gs.ui = new UI(m_gs.renderer, { WIN_WIDTH, WIN_HEIGHT });
}


/*
 * This is the place where we actually append our scenes of the game
 */

void Game::init_scenes() {
	m_gs.scene_mgr->add_scene<TestScene>(SCENE_TEST, m_gs);
	m_gs.scene_mgr->add_scene<GameScene>(SCENE_GAME, m_gs);
}


/*
 * Here we initialize our systems that are in the game
 */

void Game::init_systems() {
	m_gs.sprt_mgr = new SpriteManager();

	auto sprite_id = PLAYER;
	auto init_anim = PLAYER_IDLE;

	m_gs.anim_mgr = new Animator(PLAYER, PLAYER_IDLE);
}


/*
 * Any resources that game requires are initialized here
 */

void Game::init_resources() {
	m_gs.font_regular = new Font("./assets/Ac437_ToshibaSat_9x8.ttf", 25);

	// loading sprites

	// Player
	Sprite player_sprite = {
		.id = PLAYER,
		.path = "./assets/player.png",
		.x_cnt = 1,
		.y_cnt = 1,
	};
	m_gs.sprt_mgr->add_sprite(player_sprite, PLAYER);

	// Rock
	Sprite rock_sprite = {
		.id = ROCK,
		.path = "./assets/rock.png",
		.x_cnt = 1,
		.y_cnt = 1,
	};
	m_gs.sprt_mgr->add_sprite(rock_sprite, ROCK);
}


/*
 * This function binds the resources that the game gonna use
 */

void Game::use_resources() {
	m_gs.font_regular->bind();
}
