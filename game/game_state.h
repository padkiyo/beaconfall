#pragma once

#include "core.h"
#include "systems/dialog_system/dialog_system.h"
#include "systems/notebook_system/notebook_system.h"
#include "systems/map_system/map_system.h"


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

	// Systems
	DialogSystem* ds;
	NotebookSystem* ns;
	MapManager* mm;

	// Resources
	Font* font_regular;
};

void gs_init_core();
void gs_init_scenes();
void gs_init_systems();
void gs_init_resources();
void gs_use_resources();
void gs_free();
