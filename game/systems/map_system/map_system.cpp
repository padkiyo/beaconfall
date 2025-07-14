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

void Map::pre_calc_collisions() {
	auto layers = this->root["layers"];

	for (Json::Value::ArrayIndex layer = (layers.size() -1); layer != -1; layer--)
	{
		if(layers[layer]["name"].asString() == "COLLISION")
		{
			auto tiles = layers[layer]["tiles"];

			for (Json::Value::ArrayIndex i = 0; i != tiles.size(); i++)
			{
				glm::vec2 render_size = glm::vec2(
						this->tile_size * this->render_scale,
						this->tile_size * this->render_scale
				);

				glm::vec3 position = glm::vec3(
						tiles[i]["x"].asFloat() * render_size.x,
						(this->res.y - (tiles[i]["y"].asFloat() * render_size.y)),
						0.0f
				);

				this->boxes->push_back(
						Rect({
							position.x,
							position.y,
							render_size.x,
							render_size.y
						})
				);
			}
		}
	}
}

Map::Map(MapEntry map_config) {
	this->map_tileset = map_config.map_tileset;
	this->map_file = map_config.map_file;
	this->render_scale = map_config.render_scale;
	this->res = map_config.res;
	this->boxes = map_config.boxes;

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
	this->pre_calc_collisions();

}

Map::~Map() {
	// Freeing shit ig
	this->map_texture->clear();
}

void Map::render() {
	// Clearing the quads before rendering
	this->quads.clear();

	auto layers = this->root["layers"];

	this->map_texture->bind();

	for (Json::Value::ArrayIndex layer = (layers.size() -1); layer != -1; layer--) {
		auto tiles = layers[layer]["tiles"];
		if(layers[layer]["name"].asString() != "COLLISION")
		{
			for (Json::Value::ArrayIndex i = 0; i != tiles.size(); i++)
			{

				// Calculating map tile size from render_scale
				glm::vec2 render_size = glm::vec2(
						this->tile_size * this->render_scale,
						this->tile_size * this->render_scale
						);

				glm::vec3 position = glm::vec3(
						tiles[i]["x"].asFloat() * render_size.x,
						(this->res.y - (tiles[i]["y"].asFloat() * render_size.y)),
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

				this->quads.push_back(Quad {
						.pos = position,
						.size = render_size,
						.rot = glm::mat4(1),
						.texture = this->map_texture,
						.uv = tex_coords,
						.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
						});
			}
		}
	}
}


