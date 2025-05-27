#include "scene.h"

SceneManager* sm_create() {
	SceneManager* sm = new SceneManager;
	sm->current_scene = -1;
	return sm;
}

void sm_destroy(SceneManager* sm) {
	delete sm;
}

void sm_add_scene(
	SceneManager* sm, i32 id,
	void (*enter)(void*),
	void (*exit)(void*),
	void (*update)(void*, f64),
	void (*event)(void*, SDL_Event, f64),
	void* data
) {
	Scene* ns = new Scene;
	ns->data = data;
	ns->enter = enter;
	ns->exit = exit;
	ns->update = update;
	ns->event = event;

	sm->scenes.insert_or_assign(id, ns);
}

void sm_remove_scene(SceneManager* sm, i32 id) {
	panic(sm->scenes.find(id) != sm->scenes.end(), "Scene with ID: %d is not found", id);

	Scene* scene = sm->scenes[id];
	delete scene;

	sm->scenes.erase(id);

	// TODO: Handle if the id is the current scene
}

void sm_switch_scene(SceneManager* sm, i32 id) {
	panic(sm->scenes.find(id) != sm->scenes.end(), "Scene with ID: %d is not found", id);
	if (id == sm->current_scene) return;

	Scene* scene;

	// Exit the current scene
	if (sm->current_scene >= 0) {
		scene = sm->scenes[sm->current_scene];
		scene->exit(scene->data);
	}

	// Switching to new scene
	sm->current_scene = id;
	scene = sm->scenes[sm->current_scene];
	scene->enter(scene->data);
}

void sm_update_scene(SceneManager* sm, f64 dt) {
	Scene* scene = sm->scenes[sm->current_scene];
	scene->update(scene->data, dt);
}

void sm_handle_event(SceneManager* sm, SDL_Event event, f64 dt) {
	Scene* scene = sm->scenes[sm->current_scene];
	scene->event(scene->data, event, dt);
}

