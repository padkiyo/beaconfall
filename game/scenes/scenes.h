#pragma once

#include "core.h"

#include "dialog/dialog.h"
#include "map/map.h"
#include "notebook/notebook.h"

enum Scenes {
	SCENE_DIALOG,
	SCENE_MAP,
	SCENE_NOTEBOOK,
};

static std::string scene_name(i32 s) {
	switch (s) {
		case SCENE_DIALOG: return "Dialog Scene";
		case SCENE_MAP: return "Map Scene";
		case SCENE_NOTEBOOK: return "Notebook Scene";
		default:
			panic(false, "Unhandled scene id: %d", s);
	}
}

