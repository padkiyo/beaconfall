
#pragma once

#include "common.h"
#include "core.h"

struct MapEntry {
	const char* map_file;
	const char* map_tileset;
	f32 render_scale;

	glm::vec2 res;
	std::vector<Rect>* boxes;
};

class Map {
	private:
		f32 tile_size;
		f32 map_width;
		f32 map_height;
		f32 render_scale;

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

		std::vector<Quad> quads;

	private:
		glm::vec2 res;
		std::vector<Rect>* boxes;

	public:
		Map(MapEntry map_config);
		~Map();

		void render();
		inline const std::vector<Quad>& get_quads() const { return quads; }

	private:
		void pre_calc_collisions();
};
