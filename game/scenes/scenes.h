#pragma once

#include "core.h"

enum Scenes {
	SCENE_DIALOG,
	SCENE_MAP
};

static std::string scene_name(i32 s) {
	switch (s) {
		case SCENE_DIALOG: return "Dialog Scene";
		case SCENE_MAP: return "Map Scene";
		default:
			panic(false, "Unhandled scene id: %d", s);
	}
}

