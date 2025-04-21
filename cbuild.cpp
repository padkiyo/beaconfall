#define CBUILD_IMPLEMENTATION
#include "cbuild.h"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
	cbuild_rebuild(argc, argv);

	CBuild build("g++");
	build
		.out("bin", "game")
		.flags({
			"-std=c++20",
			"-Wl,-rpath,'$ORIGIN'",
		})
		.inc_paths({
			".",
			"./game/",
		})
#if defined(__linux__)
		.libs({
			"GL",
			"GLU",
			"m",
			"SDL2",
		})
#elif defined(_WIN32)
		.libs({
			// TODO: Add libs for windows
		})
#endif
		.src({
			"game/main.cpp",
		})
		.build()
		.clean();

#if defined(__linux__)
	std::string sdl2_src = "./vendors/SDL2/lib/libSDL2-2.0.so.0";
	std::string sdl2_dest = "./bin/libSDL2-2.0.so.0";
	LOG("Copying: %s -> %s", sdl2_src.c_str(), sdl2_dest.c_str());
	fs::copy_file(sdl2_src, sdl2_dest, fs::copy_options::overwrite_existing);
#elif defined(_WIN32)
	// TODO: Implement copying for windows
#endif

	return 0;
}
