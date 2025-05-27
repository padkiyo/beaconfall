#include "game_state.h"
#include "config.h"
#include "scenes/scenes.h"

#include "systems/dialog_system/dialog_system.h"
#include "systems/dialog_system/dialogs.h"
#include "systems/notebook_system/notebook_system.h"
#include "systems/map_system/map_system.h"
#include "systems/map_system/maps.h"

// Global game state
GameState gs;

void gs_init_core() {

	// Intializing the window
	gs.window = window_create(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE).unwrap();

	// Initializing the imgui context
	imgui_create(gs.window->sdl_window, gs.window->gl_context);

	// Initializing audio layer
	gs.audio = audio_create().unwrap();

	// Initializing quad render pipeline
	gs.quad_rp = rp_create(&QuadRendererSpecs).unwrap();
	init_texture_samples(gs.quad_rp);

	// Initializing orthographic camera
	gs.camera = camera_create(glm::vec3(0, 0, 0), {
		.left = 0,
		.right = WIN_WIDTH,
		.top = 0,
		.bottom = WIN_HEIGHT,
		.near = -1.0f,
		.far = 1000.0f
	});

	// Initializing the frame controller
	gs.fc = fc_create(FPS);
}

void gs_init_scenes() {
	gs.sm = sm_create();

	sm_add_scene(
		gs.sm, SCENE_DIALOG,
		dialog_entry,
		dialog_exit,
		dialog_update,
		dialog_event,
		NULL
	);

	sm_add_scene(
		gs.sm, SCENE_MAP,
		map_entry,
		map_exit,
		map_update,
		map_event,
		NULL
	);

	sm_add_scene(
		gs.sm, SCENE_NOTEBOOK,
		notebook_entry,
		notebook_exit,
		notebook_update,
		notebook_event,
		NULL
	);

	sm_add_scene(
		gs.sm, SCENE_SLOWMO,
		slowmo_entry,
		slowmo_exit,
		slowmo_update,
		slowmo_event,
		(void*) slowmo_create()
	);

	sm_switch_scene(gs.sm, SCENE_SLOWMO);
}

void gs_init_systems() {
	// Initializing dialog system
	gs.ds = dialog_system_create();
	dialog_system_init_dialogs(gs.ds);

	// Initializing notebook system
	gs.ns = notebook_system_create();

	// Initializing map manager
	gs.mm = mm_create();

	// Adding maps
	mm_add_map(
		gs.mm, TEST_MAP,
		"./assets/maps/test_map/map.json",
		"./assets/maps/test_map/spritesheet.png"
	).unwrap();

	mm_add_map(
		gs.mm, TEST2_MAP,
		"./assets/maps/test2_map/map.json",
		"./assets/maps/test2_map/spritesheet.png"
	).unwrap();
}

void gs_init_resources() {
	gs.font_regular = font_create("./assets/Ac437_ToshibaSat_9x8.ttf", 25).unwrap();
}

void gs_use_resources() {
	font_bind(gs.font_regular);
}

void gs_free() {
	font_destroy(gs.font_regular);

	mm_destroy(gs.mm);
	notebook_system_destroy(gs.ns);
	dialog_system_destroy(gs.ds);

	sm_destroy(gs.sm);
	camera_destroy(gs.camera);
	audio_destroy(gs.audio);
	rp_destroy(gs.quad_rp);
	window_destroy(gs.window);
}
