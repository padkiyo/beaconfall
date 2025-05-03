#pragma once

#include "core.h"
#include "systems/dialog_system/dialog_system.h"


/*
 * This is the global state of the game.
 * To excess this state use:
 * extern GameState gs;
 * in your cpp file
 *
 * The lifetime of these variables will be throught the game lifetime
 */

struct GameState {
	// Core
	Window* window;
	RenderPipeline* quad_rp;
	Audio* audio;
	Camera* camera;
	SceneManager* sm;
	DialogSystem* ds;

	// Resources
	Font* font_regular;

};
