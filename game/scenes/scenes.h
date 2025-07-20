#pragma once

#include "core.h"

#include "test/test.h"
#include "game_scene/game_scene.h"

enum Scenes {
	SCENE_TEST,
	SCENE_GAME,
};

static std::string scene_name(i32 s) {
	switch (s) {
		case SCENE_TEST: return "Test Scene";
		case SCENE_GAME: return "Game Scene";
		default:
			panic(false, "Unhandled scene id: %d", s);
	}
}

