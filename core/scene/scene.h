#pragma once

#include "common.h"

struct Scene {
	void* data;

	// Scene function pointers
	void (*enter)(void* data);                            // Called once when we enter the scene
	void (*exit)(void* data);                             // Called once when we exit the scene
	void (*update)(void* data, f64 dt);                   // Called every frame
	void (*event)(void* data, SDL_Event event, f64 dt);   // Called when event is occured in the scene
};

struct SceneManager {
	i32 current_scene;
	std::unordered_map<i32, Scene*> scenes;
};

void sm_add_scene(
	SceneManager* sm, i32 id,
	void (*enter)(void*),
	void (*exit)(void*),
	void (*update)(void*, f64),
	void (*event)(void*, SDL_Event, f64),
	void* data
);
void sm_remove_scene(SceneManager* sm, i32 id);
void sm_switch_scene(SceneManager* sm, i32 id);
void sm_update_scene(SceneManager* sm, f64 dt);
void sm_handle_event(SceneManager* sm, SDL_Event event, f64 dt);
