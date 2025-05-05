#pragma once

#include "common.h"

/*
 * To create a scene, copy paste the following function in your some_scene.h file
 * Make sure to rename the `name` prefix to your scene name.
 *
   void name_entry(void* data);
   void name_exit(void* data);
   void name_update(void* data, f64 dt);
   void name_event(void* data, SDL_Event event, f64 dt);
 */

struct Scene {
	void* data;

	// Scene function pointers
	void (*enter)(void* data);                            // Called once when we enter the scene
	void (*exit)(void* data);                             // Called once when we exit the scene
	void (*update)(void* data, f64 dt);                   // Called every frame
	void (*event)(void* data, SDL_Event event, f64 dt);   // Called when event is occured in the scene
};

struct SceneManager {
	i32 current_scene;  // NOTE: Make sure the other scenes id are not -1
	std::unordered_map<i32, Scene*> scenes;
};

SceneManager sm_create();
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
