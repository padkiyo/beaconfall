#pragma once

#include "core.h"

struct Map {
	Json::Value root;
	Texture tilesheet;
};

glm::vec4 map_get_texcoords(f32 id, f32  width, f32 height);
Result <Map, const char*> map_load(const char* json, const char* tilesheet);
void map_render(RenderPipeline* quad_rp, Map* map, f32 size);



