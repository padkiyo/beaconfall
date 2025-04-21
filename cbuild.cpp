#define CBUILD_IMPLEMENTATION
#include "cbuild.h"

int main(int argc, char** argv) {
	cbuild_rebuild(argc, argv);

	CBuild build("g++");
	build
		.out("bin", "game")
		.flags({
			"-std=c++20",
		})
		.inc_paths({
			".",
			"./game/",
		})
		.libs({
			"GL",
			"GLU",
			"m",
		})
		.src({
			"game/main.cpp",
		})
		.build()
		.clean();

	return 0;
}
