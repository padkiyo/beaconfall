#include "test.h"

TestScene::TestScene(const GameState& gs)
	: gs(gs) {
	MapEntry map_config= {
		.map_file = "assets/maps/EXAMPLE/map.json",
		.map_tileset = "assets/maps/EXAMPLE/spritesheet.png",
		.light_file = "assets/maps/EXAMPLE/light.json",
		.render_scale = 1.50f,
		.res = glm::vec2(WIN_WIDTH, WIN_HEIGHT),
		.boxes = &boxes,
		.lights = &lights,
		.quads = &m_quads
	};

	map = new Map(map_config);
}

TestScene::~TestScene() {
	delete map;
}

void TestScene::on_enter() {
	log_info("Test scene is entered\n");
}

void TestScene::on_exit() {
	log_info("Test scene is exited\n");
}

void TestScene::on_event(const SDL_Event& event, f64 dt) {
}

void TestScene::on_update(f64 dt) {

	// Setting up the lights
	set_ambient_color(ambient_color);
	set_light_pixel_size({pixel_size, pixel_size});

	// Rendering the map
	map->render();

	if(show_collision){
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
	}

	// Adding lights to the scene
	for (auto light: lights){
		add_light(light);
	}
}

void TestScene::on_ui_render(UI& ui) {
	ui.text("Hello world", { 400, 300 }, Style {
		.fg_color = { 1, 1, 1, 1 },
		.font = gs.font_regular,
	});

	if (ui.button("Click Me", Rect({ 100, 100, 200, 50 }), Style {.font = gs.font_regular})) {
		std::cout << "Clicked" << std::endl;
	}
}

void TestScene::on_imgui_render() {
	if (ImGui::CollapsingHeader("Setting")) {
	}

	if (ImGui::CollapsingHeader("Light")) {
		ImGui::ColorEdit3("Ambient Color", (f32*) &ambient_color);
		ImGui::DragFloat("Pixel Size", &pixel_size, 0.1f, 1.0f, 50.0f);

		i32 count = 0;
		for (auto& light: lights){
			std::string seperator_text = std::format("Light {}",  count);
			std::string position = std::format("Position{}", count);
			std::string radius = std::format("Radius{}", count);
			std::string intensity = std::format("Intensity{}", count);
			std::string direction = std::format("Direction{}", count);
			std::string fov = std::format("FOV{}", count);
			std::string color = std::format("Color{}", count);

			ImGui::SeparatorText(seperator_text.c_str());
			ImGui::DragFloat2(position.c_str(),(float*)&light.pos, 1.0f, -500.0f, 500.0f);
			ImGui::DragFloat(radius.c_str(), &light.radius, 0.5f, 0.0f, 50.0f);
			ImGui::DragFloat(intensity.c_str(), &light.intensity, 0.01f, 0.0f, 10.0f);
			ImGui::SliderAngle(direction.c_str(), &light.dir, 0.0f, 360.0f);
			ImGui::SliderAngle(fov.c_str(), &light.fov, 0.0f, 360.0f);
			ImGui::ColorEdit4(color.c_str(), (float*)&light.color);
			count += 1;
		}
		if(ImGui::Button("Bake lights")){
			map->write_light_file();
		}
	}

	if(ImGui::CollapsingHeader("Map")){
		ImGui::Checkbox("show hit box", &show_collision);
	}
}
