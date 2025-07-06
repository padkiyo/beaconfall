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

void SceneManager::handle_event(const SDL_Event& event, f64 dt) {
	panic(m_curr_scene >= 0, "No current scene is found.");

	Scene* scene = m_scenes[m_curr_scene];
	scene->on_event(event, dt);
}
