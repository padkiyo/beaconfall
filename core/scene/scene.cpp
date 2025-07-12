#include "scene.h"

/*
 * Scene Implementations
 */

void Scene::clear() {
	m_quads.clear();
	m_lights.clear();
}

void Scene::add_quad(const Quad& quad) {
	m_quads.push_back(quad);
}

void Scene::add_light(const Light& light) {
	panic(m_lights.size() + 1 < MAX_LIGHTS, "To much light in one scene");
	m_lights.push_back(light);
}

void Scene::set_light_pixel_size(const glm::vec2& size) {
	m_light_pixel_size = size;
}

void Scene::set_ambient_color(const glm::vec3& color) {
	m_ambient_color = color;
}


/*
 * Scene Manager Implementations
 */

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
	scene->clear();
	scene->on_update(dt);
}

void SceneManager::render_current_scene_ui(UI& ui) {
	panic(m_curr_scene >= 0, "No current scene is found.");

	Scene* scene = m_scenes[m_curr_scene];
	scene->on_ui_render(ui);
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
