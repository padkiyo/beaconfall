// TODO: better error handling by saying which file is not loading
#include "map_system.h"

// ! was too high when I wrote this, now I dont understand how this shit works, it works tho!
glm::vec4  Map::get_texcoords(f32 index, f32 width, f32 height) {
	glm::vec4 res = glm::vec4(0.0f, 0.0f,
			0.0f, 0.0f);

	res.x = ((i32) index % (i32)width)/width;

	f32 adder = (height - 1)/height;

	i32 cofficient = (int) (index/width);
	f32 subtractor = cofficient/ height;

	res.y = adder - subtractor;

	res.z =  1/width;
	res.w = 1/height;

	return res;
}

Map::Map(MapEntry map_config) {

	this->renderer = map_config.renderer;
	this->map_tileset = map_config.map_tileset;
	this->map_file = map_config.map_file;
	this->render_scale = map_config.render_scale;

	// loading map file
	Json::CharReaderBuilder builder;
	std::ifstream ifs;
	JSONCPP_STRING errs;

	ifs.open(this->map_file);
	builder["collectComments"] = false;

	if(!parseFromStream(builder, ifs, &this->root, &errs))
		panic("Unable to open map a file");

	this->tile_size = this->root["tileSize"].asFloat();
	this->map_width = this->root["mapWidth"].asFloat();
	this->map_height = this->root["mapHeight"].asFloat();

	this->map_texture = new Texture(this->map_tileset, this->sprite_filter);

}

void Map::render() {
	auto layers = this->root["layers"];

	this->map_texture->bind();

	for (Json::Value::ArrayIndex layer = (layers.size() -1); layer != -1; layer--) {
		auto tiles = layers[layer]["tiles"];

		for (Json::Value::ArrayIndex i = 0; i != tiles.size(); i++)
		{

			// Calculating map tile size from render_scale
			glm::vec2 render_size = glm::vec2(
					this->tile_size * this->render_scale,
					this->tile_size * this->render_scale
			);

			glm::vec3 position = glm::vec3(
					tiles[i]["x"].asFloat() * render_size.x,
					(this->renderer->m_res.y - (tiles[i]["y"].asFloat() * render_size.y)),
					0.0f
			);

			f32 sprite_sheet_width_count = (f32) this->map_texture->get_width()/this->tile_size;
			f32 sprite_sheet_height_count = (f32) this->map_texture->get_height()/this->tile_size;

			// Scary stuff pls find a better solution
			f32 tile_id = std::stof(tiles[i]["id"].asString());

			glm::vec4  tex_coords = this->get_texcoords(
				tile_id,
				sprite_sheet_width_count,
				sprite_sheet_height_count
			);

			// NOTE just in case for debugging
			//std::cout << "P: " <<  position.x << "," << position.y << " TC: " << tex_coords.x << "," << tex_coords.y << "," << tex_coords.z << "," << tex_coords.w << std::endl;

			this->renderer->push_quad(
					position, render_size, // position and size of quad
					glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), {0,0,1}), // rotation stuff
					(Texture&)*this->map_texture, tex_coords, // texture stuff
					glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) // color stuff
				);
		}
	}
}


