#pragma once

#include "common.h"
#include "renderer/geometry.h"

/*
 * This is a Pure Virtual Class for creating Scenes
 * To create a new scene just inheriet this class and define all these functions
 */

class Scene {
public:
	virtual void on_enter() = 0;                                // Called when the scene is entered
	virtual void on_exit() = 0;                                 // Called when the scene is exited
	virtual void on_update(f64 dt) = 0;                         // Called every frame
	virtual void on_event(const SDL_Event& event, f64 dt) = 0;  // Called when an event is triggered
	virtual void on_imgui_render() = 0;                         // Called inside the imgui rendering block

	// Commands in scene
	void clear();
	void add_quad(const Quad& quad);
	void add_light(const Light& light);
	void set_light_pixel_size(const glm::vec2& size);
	void set_ambient_color(const glm::vec3& color);

	// Getters
	inline const std::vector<Quad>& get_quads() const { return m_quads; }
	inline const std::vector<Light>& get_lights() const { return m_lights; }
	inline const glm::vec2& get_light_pixel_size() const { return m_light_pixel_size; }
	inline const glm::vec3& get_ambient_color() const { return m_ambient_color; }

protected:
	std::vector<Quad> m_quads;
	std::vector<Light> m_lights;
	glm::vec2 m_light_pixel_size = { 2, 2 };
	glm::vec3 m_ambient_color = { 0.2, 0.2, 0.2 };
};


/*
 * Scene Manager handles the owning and switching of the scenes in game
 * This holds the actual scenes and is responsible for calling the respective scene functions
 */

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	const std::unordered_map<i32, Scene*>& get_scenes() const {
		return m_scenes;
	}
	inline i32 get_current_scene_id() const { return m_curr_scene; }
	inline const Scene& get_current_scene() {
		return *m_scenes[m_curr_scene];
	}

	template<typename T, typename... Args>
	void add_scene(i32 id, Args&&... args) {
		panic(id >= 0, "Scene ID cannot be -ve");

		// Constructing the scene with the arguments
		T* scene = new T(std::forward<Args>(args)...);

		// Storing the scene
		m_scenes.insert_or_assign(id, scene);
	}

	void remove_scene(i32 id);
	void switch_scene(i32 id);
	void update_current_scene(f64 dt);
	void update_imgui_render();
	void handle_event(const SDL_Event& event, f64 dt);

private:
	i32 m_curr_scene = -1;

	// The scene manager owns the Scenes
	// (ie responsible for allocation and deallocating the scenes)
	std::unordered_map<i32, Scene*> m_scenes;
};
