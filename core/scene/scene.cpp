#include "scene.h"


SceneManager::SceneManager() {}

SceneManager::~SceneManager() {
	for (auto it: m_scenes) {
		delete it.second;
	}
}

void SceneManager::remove_scene(i32 id) {
	panic(
		m_scenes.find(id) != m_scenes.end(),
		"Scene with ID: %d is not found", id
	);

	// Deleting the scene
	Scene* scene = m_scenes[id];
	m_scenes.erase(id);
	delete scene;

	if (m_curr_scene != id) return;

	// If the removed scene was the current scene
	if (m_scenes.empty()) {
		m_curr_scene = -1;
	} else {
		m_curr_scene = m_scenes.begin()->first;
	}
}

void SceneManager::switch_scene(i32 id) {
	panic(
		m_scenes.find(id) != m_scenes.end(),
		"Scene with ID: %d is not found", id
	);

	if (id == m_curr_scene) return;

	Scene* scene;

	// Exit the current scene
	if (m_curr_scene >= 0) {
		scene = m_scenes[m_curr_scene];
		scene->on_exit();
	}

	// Switching to new scene
	m_curr_scene = id;
	scene = m_scenes[m_curr_scene];
	scene->on_enter();
}

void SceneManager::update_current_scene(f64 dt) {
	panic(m_curr_scene >= 0, "No current scene is found.");

	Scene* scene = m_scenes[m_curr_scene];
	scene->on_update(dt);
}

void SceneManager::update_imgui_render() {
	panic(m_curr_scene >= 0, "No current scene is found.");

	Scene* scene = m_scenes[m_curr_scene];
	scene->on_imgui_render();
}

void SceneManager::handle_event(SDL_Event event, f64 dt) {
	panic(m_curr_scene >= 0, "No current scene is found.");

	Scene* scene = m_scenes[m_curr_scene];
	scene->on_event(event, dt);
}

/*
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
*/
