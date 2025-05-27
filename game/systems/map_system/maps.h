#pragma once

#include "core.h"

enum Maps{
	TEST_MAP,
	TEST2_MAP
};


static std::string map_name(i32 s) {
	switch (s) {
		case TEST_MAP: return "Test Map";
		case TEST2_MAP: return "Test2 Map";
		default:
			panic(false, "Unhandled map id: %d", s);
	}
}
