#pragma once

#include "common.h"
#include "core.h"


struct Map {
	Json::Value root;
	Texture tilesheet;
};

struct  MapManager {
	i32 current_map;
	std::unordered_map<i32, Map*> maps;
};


MapManager* mm_create();
void mm_destroy(MapManager* mm);

Result<i32 , const char*> mm_add_map(
	MapManager* mm, i32 id,
	const char* json,
	const char* tilesheet
);

void mm_switch_map(MapManager* mm, i32 id);
void mm_remove_map(MapManager* mm, i32 id);
void mm_render_current(RenderPipeline* quad_rp, MapManager* mm, f32 size);


glm::vec4 map_get_texcoords(f32 id, f32  width, f32 height);
Result <Map, const char*> map_load(const char* json, const char* tilesheet);
void map_render(RenderPipeline* quad_rp, Map* map, f32 size);



