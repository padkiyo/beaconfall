
#pragma once

#include "common.h"
#include "core.h"

struct MapEntry {
	const char* map_file;
	const char* map_tileset;
	Renderer* renderer;
	f32 render_scale;
};

class Map {
	private:
		f32 tile_size;
		f32 map_width;
		f32 map_height;
		f32 render_scale;

		Renderer* renderer;
		const char* map_tileset;
		const char* map_file;
		Json::Value root;

		Texture* map_texture;

		TextureFilter sprite_filter = {
			.min_filter = GL_NEAREST,
			.mag_filter = GL_NEAREST,
			.wrap_s = GL_CLAMP_TO_EDGE,
			.wrap_t = GL_CLAMP_TO_EDGE,
			.flip = true
		};

		glm::vec4 get_texcoords(f32 index, f32 width, f32 height);

	public:
		Map(MapEntry map_config);
		~Map();

		void render();
};
