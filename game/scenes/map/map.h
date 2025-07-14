#pragma once

#include "core.h"
#include "game.h"

class MapScene : public Scene {
public:
	MapScene(const GameState& gs)
		: m_gs(gs)
	{}
	~MapScene() {}

public:
	void on_enter();
	void on_exit();
	void on_update(f64 dt);
	void on_event(const SDL_Event& event, f64 dt);
	void on_imgui_render();

private:
	const GameState& m_gs;
};
