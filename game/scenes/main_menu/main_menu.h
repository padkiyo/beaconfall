#pragma once

#include "core.h"
#include "config.h"
#include "game.h"

class MainMenu : public Scene {
public:
	MainMenu(GameState& gs);
	~MainMenu();

public:
	void on_enter();
	void on_exit();
	void on_event(const SDL_Event& event, f64 dt);
	void on_update(f64 dt);
	void on_ui_render(UI& ui);
	void on_imgui_render();

private:
	GameState& m_gs;
};
