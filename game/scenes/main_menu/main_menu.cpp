#include "main_menu.h"
#include "scenes/scenes.h"

MainMenu::MainMenu(GameState& gs) : m_gs(gs) {
}

MainMenu::~MainMenu() {
}

void MainMenu::on_enter() {
}

void MainMenu::on_exit() {
}

void MainMenu::on_event(const SDL_Event& event, f64 dt) {
}

void MainMenu::on_update(f64 dt) {
}

void MainMenu::on_ui_render(UI& ui) {
	std::string title = "Game";
	glm::vec2 size_1 = m_gs.font_regular->calculate_dim(title);
	ui.text(
		title,
		{WIN_WIDTH / 2 - size_1.x / 2, WIN_WIDTH / 2 - size_1.y / 2},
		Style {
			.font = m_gs.font_regular,
		}
	);

	std::string label_1 = "Play";
	glm::vec2 size_2 = m_gs.font_regular->calculate_dim(label_1);
	glm::vec2 bs_2 = { 200, 50 }; 
	Rect r1({
		WIN_WIDTH / 2 - bs_2.x / 2,
		200 - bs_2.y / 2,
		bs_2.x, bs_2.y
	});
	if (ui.button(label_1, r1, Style { .font = m_gs.font_regular })) {
		m_gs.scene_mgr->switch_scene(SCENE_GAME);
	}

	std::string label_2 = "Quit";
	glm::vec2 size_3 = m_gs.font_regular->calculate_dim(label_2);
	Rect r2({
		WIN_WIDTH / 2 - bs_2.x / 2,
		130 - bs_2.y / 2,
		bs_2.x, bs_2.y
	});
	if (ui.button(label_2, r2, Style { .font = m_gs.font_regular })) {
		m_gs.running = false;
	}
}

void MainMenu::on_imgui_render() {
}
