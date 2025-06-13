#pragma once

#include "core.h"

#include "dialog/dialog.h"
#include "map/map.h"
#include "notebook/notebook.h"
#include "slowmo/slowmo.h"
#include "test/test.h"

enum Scenes {
	SCENE_DIALOG,
	SCENE_MAP,
	SCENE_NOTEBOOK,
	SCENE_SLOWMO,
	SCENE_TEST,
};

static std::string scene_name(i32 s) {
	switch (s) {
		case SCENE_DIALOG: return "Dialog Scene";
		case SCENE_MAP: return "Map Scene";
		case SCENE_NOTEBOOK: return "Notebook Scene";
		case SCENE_SLOWMO: return "Slowmo Scene";
		case SCENE_TEST: return "Test Scene";
		default:
			panic(false, "Unhandled scene id: %d", s);
	}
}

