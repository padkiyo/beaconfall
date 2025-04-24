#define CBUILD_IMPLEMENTATION
#include "cbuild.h"
#include <chrono>

namespace fs = std::filesystem;

// The list of libraries to be copied from vendors to the executable directory
std::unordered_map<std::string, std::string> linux_libs = {
	{ "./vendors/SDL2/lib/libSDL2-2.0.so.0", "./bin/libSDL2-2.0.so.0" },
	{ "./vendors/SDL2/lib/libSDL2_mixer-2.0.so.0", "./bin/libSDL2_mixer-2.0.so.0" },
};

int main(int argc, char** argv) {
	cbuild_rebuild(argc, argv);

	auto start = std::chrono::high_resolution_clock::now();

	// This holds the file name with its updated time
	FileRecords records;
	read_file_records(records);

	// Building the C vendors
	CBuild build_c("gcc");
	build_c
		.set_file_records(&records)
		.src({
			"vendors/glad/src/gl.c",
			"vendors/stb/stb_image.c",
		})
		.inc_paths({
			"vendors/glad/include",
			"vendors/stb",
		})
		.compile();

	// Final build
	CBuild build("g++");
	build
		.set_file_records(&records)
		.out("bin", "game")
		.flags({
			"-std=c++20",
			"-Wl,-rpath,'$ORIGIN'",
		})
		.inc_paths({
			".",
			"./vendors/glad/include",
			"./vendors/imgui/include",
			"./vendors/stb",
			"./vendors/glm",
			"./core",
			"./game/",
		})
#if defined(__linux__)
		.libs({
			"GL",
			"GLU",
			"m",
			"SDL2",
			"SDL2_mixer",
		})
#elif defined(_WIN32)
		.libs({
			// TODO: Add libs for windows
		})
#endif
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

			"./core/window/window.cpp",
			"./core/error/error.cpp",
			"./core/audio/audio.cpp",
			"./core/imgui/core_imgui.cpp",
			"./core/shader/shader.cpp",
			"./core/texture/texture.cpp",
			"./core/renderer/renderer.cpp",
			"./core/camera/camera.cpp",

			"./game/main.cpp",
		})
		.build();

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

	save_file_records(records);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "[LOG]: Build took: " << duration.count() << " seconds" << std::endl;

	return 0;
}
