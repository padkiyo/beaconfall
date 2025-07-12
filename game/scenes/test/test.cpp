#include "test.h"

TestScene::TestScene(const GameState& gs)
	: gs(gs) {
	MapEntry map_config= {
		.map_file = "assets/maps/EXAMPLE/map.json",
		.map_tileset = "assets/maps/EXAMPLE/spritesheet.png",
		.render_scale = 1.50f
	};
	map = new Map(map_config);
}

TestScene::~TestScene() {
	delete map;
}

void TestScene::on_enter() {
	log_info("Test scene is entered\n");

	l1 = {
		.pos = {130, 197},
		.radius = 0.5f,
		.intensity = 0.45f,
		.dir = glm::radians(81.0f) ,
		.fov = glm::radians(41.0f),
		.color = {0.8,0.5,0,1}
	};

	l2 = {
		.pos = {347, 225},
		.radius = 0.5f,
		.intensity = 0.610f,
		.dir = glm::radians(180.0f),
		.fov = glm::radians(360.0f),
		.color = {1.0f,0.522,0.04,1}
	};

	a = Rect({400, 350, 50, 50});
	boxes = {
		Rect({10, 10, 200, 20}),
		Rect({10, 20, 20, 200}),
		Rect({200, 10, 200, 20})
	};
}

void TestScene::on_exit() {
	log_info("Test scene is exited\n");
}

void TestScene::on_event(const SDL_Event& event, f64 dt) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
			case SDLK_w:
				move.y += move_speed;
				break;
			case SDLK_s:
				move.y -= move_speed;
				break;

			case SDLK_a:
				move.x -= move_speed;
				break;
			case SDLK_d:
				move.x += move_speed;
				break;
		}
	}
}

void TestScene::on_update(f64 dt) {
	// Setting up the lights
	set_ambient_color(ambient_color);
	set_light_pixel_size({pixel_size, pixel_size});

	// Rendering the map
	map->render({ WIN_WIDTH, WIN_HEIGHT });

	// Getting the quads from map and adding to the scene
	const std::vector<Quad>& quads = map->get_quads();
	for (auto quad : quads) {
		add_quad(quad);
	}

	add_light(l1);
	add_light(l2);

	add_quad(Quad {
		{a.x, a.y, 0},
		{a.w, a.h},
		glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), {0,0,1}),
		&gs.sprt_mgr->get_spritesheet_texture(PLAYER),
		gs.anim_mgr->get_frame(*gs.sprt_mgr),
		{1,1,1,1}
	});

	for (auto box : boxes) {
		add_quad(Quad {
			{box.x, box.y, 0},
			{box.w, box.h},
			glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), {0,0,1}),
			&gs.renderer->white_texture(),
			{0,0,1,1},
			{1,0,1,1}
		});
	}

	a.resolve(boxes, move, gs.fc->dt());

	move = glm::vec2(0);
}

void TestScene::on_imgui_render() {
	if (ImGui::CollapsingHeader("Setting")) {
		ImGui::DragFloat("Speed", &move_speed, 10.0f, 1.0f, 1000.0f);
	}

	if (ImGui::CollapsingHeader("Light")) {
		ImGui::ColorEdit3("Ambient Color", (f32*) &ambient_color);
		ImGui::DragFloat("Pixel Size", &pixel_size, 0.1f, 1.0f, 50.0f);

		ImGui::SeparatorText("Light 1");
		ImGui::DragFloat2("Position1", (float*)&l1.pos, 1.0f, -500.0f, 500.0f);
		ImGui::DragFloat("Radius1", &l1.radius, 0.5f, 0.0f, 50.0f);
		ImGui::DragFloat("Intensity1", &l1.intensity, 0.01f, 0.0f, 10.0f);
		ImGui::SliderAngle("Direction1", &l1.dir, 0.0f, 360.0f);
		ImGui::SliderAngle("FOV1", &l1.fov, 0.0f, 360.0f);
		ImGui::ColorEdit4("Color1", (float*)&l1.color);

		ImGui::SeparatorText("Light 2");
		ImGui::DragFloat2("Position2", (float*)&l2.pos, 1.0f, -500.0f, 500.0f);
		ImGui::DragFloat("Radius2", &l2.radius, 0.5f, 0.0f, 50.0f);
		ImGui::DragFloat("Intensity2", &l2.intensity, 0.01f, 0.0f, 10.0f);
		ImGui::SliderAngle("Direction2", &l2.dir, 0.0f, 360.0f);
		ImGui::SliderAngle("FOV2", &l2.fov, 0.0f, 360.0f);
		ImGui::ColorEdit4("Color2", (float*)&l2.color);
	}
}
