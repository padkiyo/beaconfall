
#pragma once

#include "common.h"
#include "core.h"

struct MapEntry {
	const char* map_file;
	const char* map_tileset;
	const char* light_file;
	f32 render_scale;

	glm::vec2 res;
	std::vector<Rect>* boxes;
	std::vector<Light>* lights;
	std::vector<Quad>* quads;
};

class Map {
	private:
		f32 tile_size;
		f32 map_width;
		f32 map_height;
		f32 render_scale;

		const char* map_tileset;
		const char* map_file;
		const char* light_file;
		Json::Value root;
		Json::Value light_root;
		bool has_light_file = true;

		std::unordered_map<std::string, Light> light_map;

		Texture* map_texture;

		TextureFilter sprite_filter = {
			.min_filter = GL_NEAREST,
			.mag_filter = GL_NEAREST,
			.wrap_s = GL_CLAMP_TO_EDGE,
			.wrap_t = GL_CLAMP_TO_EDGE,
			.flip = true
		};

		Light default_light = {
			.pos = {0, 0},
			.radius = 0.5f,
			.intensity = 0.45f,
			.dir = glm::radians(81.0f) ,
			.fov = glm::radians(41.0f),
			.color = {0.8,0.5,0,1}
		};

		glm::vec4 get_texcoords(f32 index, f32 width, f32 height);

	private:
		glm::vec2 res;
		std::vector<Rect>* boxes;
		std::vector<Light>* lights;
		std::vector<Quad>* quads;

	public:
		Map(MapEntry map_config);
		~Map();
		void write_light_file();

		void render();

	private:
		void pre_calc_collisions();
};
