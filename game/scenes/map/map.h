#pragma once

#include "core.h"

class MapScene : public Scene {
public:
	MapScene() {}
	~MapScene() {}

public:
	void on_enter();
	void on_exit();
	void on_update(f64 dt);
	void on_event(SDL_Event event, f64 dt);
	void on_imgui_render();
};
