#pragma once

#include "core.h"
#include "game.h"
#include "config.h"
#include "structure/beacon.h"


class BeaconScene: public Scene {
public:
	BeaconScene(const GameState& gs);
	~BeaconScene();

public:
	void on_enter();
	void on_exit();
	void on_event(const SDL_Event& event, f64 dt);
	void on_update(f64 dt);
	void on_ui_render(UI& ui);
	void on_imgui_render();

private:
	const GameState& m_gs;

	std::vector<Rect> m_boxes;

	Beacon beacon = Beacon();

	TextureFilter sprite_filter = {
		.min_filter = GL_NEAREST,
		.mag_filter = GL_NEAREST,
		.wrap_s = GL_CLAMP_TO_EDGE,
		.wrap_t = GL_CLAMP_TO_EDGE,
		.flip = true
	};

	Texture map_texture = Texture("./assets/BASE_MAP.png", sprite_filter);

	// Light
	glm::vec3 ambient_color = { 1, 1, 1};
	f32 pixel_size = 2;
};

