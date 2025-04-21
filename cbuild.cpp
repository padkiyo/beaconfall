#define CBUILD_IMPLEMENTATION
#include "cbuild.h"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
	cbuild_rebuild(argc, argv);

	FileRecords records;
	read_file_records(records);

	CBuild glad_build("gcc");
	glad_build
		.set_file_records(&records)
		.src({
			"vendors/glad/src/gl.c"
		})
		.inc_paths({
			"vendors/glad/include"
		})
		.compile();

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
			"./game/",
			"./vendors/glad/include",
			"./core",
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
				"./objs/gl.o"
		})
		.src({
			"./core/window/window.cpp",
			"./core/error/error.cpp",
			"./core/audio/audio.cpp",
			"./game/main.cpp",
		})
		.build();

#if defined(__linux__)

	std::string sdl2_src = "./vendors/SDL2/lib/libSDL2-2.0.so.0";
	std::string sdl2_dest = "./bin/libSDL2-2.0.so.0";
	LOG("Copying: %s -> %s", sdl2_src.c_str(), sdl2_dest.c_str());
	fs::copy_file(sdl2_src, sdl2_dest, fs::copy_options::overwrite_existing);

	std::string mixer_src = "./vendors/SDL2/lib/libSDL2_mixer-2.0.so.0";
	std::string mixer_dest = "./bin/libSDL2_mixer-2.0.so.0";
	LOG("Copying: %s -> %s", mixer_src.c_str(), mixer_dest.c_str());
	fs::copy_file(mixer_src, mixer_dest, fs::copy_options::overwrite_existing);

#elif defined(_WIN32)
	// TODO: Implement copying for windows
#endif

	save_file_records(records);

	return 0;
}
