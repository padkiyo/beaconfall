#define CBUILD_IMPLEMENTATION
#include "cbuild.h"
#include <chrono>

namespace fs = std::filesystem;

// The list of libraries to be copied from vendors to the executable directory
std::unordered_map<std::string, std::string> linux_libs = {
	{ "./vendors/SDL2/lib/libSDL2-2.0.so.0", "./bin/libSDL2-2.0.so.0" },
	{ "./vendors/SDL2/lib/libSDL2_mixer-2.0.so.0", "./bin/libSDL2_mixer-2.0.so.0" },
	{ "./vendors/SDL2/lib/libSDL2_ttf-2.0.so.0", "./bin/libSDL2_ttf-2.0.so.0" },
};

// :core
void build_core() {
	CBuild build("g++");
	build
		.out("bin", "libcore.so")
		.flags({
			"-fPIC",
			"-shared",
			"-std=c++20",
			"-Wl,-rpath,'$ORIGIN'",
		})
		.inc_paths({
			".",
			"./vendors/glad/include",
			"./vendors/imgui/include",
			"./vendors/json",
			"./vendors/stb",
			"./vendors/glm",
			"./core",
		})
		.lib_paths({
			"./bin/",
			"./vendors/SDL2/lib/",
		})
		.libs({
			"vendors",
			"GL",
			"GLU",
			"m",
			"SDL2",
			"SDL2_mixer",
			"SDL2_ttf",
		})
		.src({
			"./core/base/utils.cpp",
			"./core/base/error.cpp",
			"./core/window/window.cpp",
			"./core/audio/audio.cpp",
			"./core/imgui/core_imgui.cpp",
			"./core/shader/shader.cpp",
			"./core/texture/texture.cpp",
			"./core/buffers/vertex_buffer.cpp",
			"./core/buffers/vertex_array.cpp",
			"./core/buffers/frame_buffer.cpp",
			"./core/renderer/renderer.cpp",
			"./core/camera/camera.cpp",
			"./core/font/font.cpp",
			"./core/scene/scene.cpp",
			"./core/frame_controller/frame_controller.cpp",
			"./core/rect/rect.cpp",
		})
		.build()
		.clean();
}

// :vendors
void build_vendors() {
	CBuild build_c("gcc");
	build_c
		.flags({
			"-fPIC",
		})
		.inc_paths({
			"vendors/glad/include",
			"vendors/stb",
		})
		.src({
			"vendors/glad/src/gl.c",
			"vendors/stb/stb_image.c",
		})
		.compile();

	CBuild build_cpp("g++");
	build_cpp
		.out("bin", "libvendors.so")
		.flags({
			"-fPIC",
			"-shared",
		})
		.inc_paths({
			".",
			"./vendors/glad/include",
			"./vendors/imgui/include",
			"./vendors/json",
			"./vendors/stb",
			"./vendors/glm",
			"./core",
		})
		.lib_paths({
			"./bin/",
			"./vendors/SDL2/lib/",
		})
		.libs({
			"GL",
			"GLU",
			"m",
			"SDL2",
			"SDL2_mixer",
			"SDL2_ttf",
		})
		.objs({
			"./objs/gl.o",
			"./objs/stb_image.o",
		})
		.src({
			"./vendors/imgui/src/imgui.cpp",
			"./vendors/imgui/src/imgui_demo.cpp",
			"./vendors/imgui/src/imgui_draw.cpp",
			"./vendors/imgui/src/imgui_impl_opengl3.cpp",
			"./vendors/imgui/src/imgui_impl_sdl2.cpp",
			"./vendors/imgui/src/imgui_tables.cpp",
			"./vendors/imgui/src/imgui_widgets.cpp",
			"./vendors/imgui/src/imgui_stdlib.cpp",
			"./vendors/json/json/json_reader.cpp",
			"./vendors/json/json/json_value.cpp",
			"./vendors/json/json/json_writer.cpp",
		})
		.build()
		.clean();
}

// :game
void build_game() {
	CBuild build("g++");
	build
		.out("bin", "game")
		.flags({
			"-std=c++20",
			"-Wl,-rpath,'$ORIGIN'",
		})
		.inc_paths({
			".",
			"./vendors/glad/include",
			"./vendors/imgui/include",
			"./vendors/json",
			"./vendors/stb",
			"./vendors/glm",
			"./core",
			"./game/",
		})
		.lib_paths({
			"./bin/",
			"./vendors/SDL2/lib/",
		})
		.libs({
			"core",
			"vendors",
			"GL",
			"GLU",
			"m",
			"SDL2",
			"SDL2_mixer",
			"SDL2_ttf",
		})
		.src({
//			"./game/systems/dialog_system/dialog_system.cpp",
//			"./game/systems/notebook_system/notebook_system.cpp",
//			"./game/systems/map_system/map_system.cpp",
			"./game/scenes/test/test.cpp",
//			"./game/scenes/map/map.cpp",
//			"./game/scenes/slowmo/slowmo.cpp",
//			"./game/scenes/notebook/notebook.cpp",
//			"./game/scenes/dialog/dialog.cpp",
			"./game/game.cpp",
			"./game/main.cpp",
		})
		.build()
		.clean();
}

void move_libraries() {
#if defined(__linux__)

	printf("\n");
	LOG("Copying over the library files.");
	for (const auto& [from, to] : linux_libs) {
		LOG("Copying: %s -> %s", from.c_str(), to.c_str());
		fs::copy_file(from, to, fs::copy_options::overwrite_existing);
	}

#elif defined(_WIN32)
	// TODO: Implement copying for windows
#endif
}

void print_usage() {
	std::cout << "[Usage]: ./cbuild [options]" << std::endl;
	std::cout << "[Options]:\n";
	std::cout << "  vendors: Builds vendors\n";
	std::cout << "  core: Builds core\n";
	std::cout << "  game: Builds game\n";
	std::cout << "  all: Builds everything\n";
}

int main(int argc, char** argv) {
	cbuild_rebuild(argc, argv);

	std::vector<std::string> args(argv, argv + argc);

	args.erase(args.begin());

	if (!args.size())
		print_usage();

	auto start = std::chrono::high_resolution_clock::now();

	for (auto& arg : args) {
		if (arg == "vendors") {
			build_vendors();
		} else if (arg == "core") {
			build_core();
		} else if (arg == "game") {
			build_game();
			move_libraries();
		} else if (arg == "all") {
			build_vendors();
			build_core();
			build_game();
			move_libraries();
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "[LOG]: Build took: " << duration.count() << " seconds" << std::endl;

	return 0;
}
