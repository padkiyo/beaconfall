#pragma once

#include "common.h"

class Scene {
public:
	virtual void on_enter() = 0;
	virtual void on_exit() = 0;
	virtual void on_update(f64 dt) = 0;
	virtual void on_event(SDL_Event event, f64 dt) = 0;
	virtual void on_imgui_render() = 0;
};

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	const std::unordered_map<i32, Scene*>& get_scenes() const {
		return m_scenes;
	}
	i32 get_current_scene() const { return m_curr_scene; }

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
	void handle_event(SDL_Event event, f64 dt);

private:
	i32 m_curr_scene = -1;

	// The scene manager owns the Scenes
	// (ie responsible for allocation and deallocating the scenes)
	std::unordered_map<i32, Scene*> m_scenes;
};



/*
 * To create a scene, copy paste the following function in your some_scene.h file
 * Make sure to rename the `name` prefix to your scene name.
 *
   void name_entry(void* data);
   void name_exit(void* data);
   void name_update(void* data, f64 dt);
   void name_event(void* data, SDL_Event event, f64 dt);
 */

/*
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

SceneManager* sm_create();
void sm_destroy(SceneManager* sm);
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
*/
