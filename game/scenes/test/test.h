#pragma once

#include "core.h"

class TestScene : public Scene {
public:
	TestScene() {}
	~TestScene() {}

public:
	void on_enter();
	void on_exit();
	void on_update(f64 dt);
	void on_event(const SDL_Event& event, f64 dt);
	void on_imgui_render();
};
