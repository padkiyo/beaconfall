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

void Map::write_light_file() {
	Json::StreamWriterBuilder builder;

	Json::Value root;
//	Json::Value lights;
//
	Json::Value light_list;
	Json::Value light_item;
	for (auto light: *this->lights) {
		light_item["x"] =(i32) light.map_pos.x;
		light_item["y"] = (i32) light.map_pos.y;
		light_item["radius"] = light.radius;
		light_item["intensity"] = light.intensity;
		light_item["direction"] = light.dir;
		light_item["fov"] = light.fov;
		light_item["r"] = light.color.x;
		light_item["g"] = light.color.y;
		light_item["b"] = light.color.z;
		light_item["a"] = light.color.w;

		light_list.append(light_item);
	}

	root["lights"] = light_list;

	// printing stuff
	std::string light_file_data = Json::writeString(builder, root);

	std::ofstream light_file(this->light_file);
	light_file << light_file_data;
	light_file.close();

	std::cout << light_file_data << std::endl;
}
void Map::pre_calc_collisions() {
	auto layers = this->root["layers"];

	if(this->has_light_file){
		auto lights = this->light_root["lights"];

		for(auto light: lights) {
			Light tmp_light= {
				.pos = {0, 0},
				.radius = light["radius"].asFloat(),
				.intensity =  light["intensity"].asFloat(),
				.dir =  light["direction"].asFloat(),
				.fov =  light["fov"].asFloat(),
				.color = { light["r"].asFloat(), light["g"].asFloat(), light["b"].asFloat(), light["a"].asFloat()}
			};
			std::string id = std::format("{},{}",light["x"].asString(), light["y"].asString());
			this->light_map[id] = tmp_light;
		}
	}

	for (Json::Value::ArrayIndex layer = (layers.size() -1); layer != -1; layer--)
	{
		auto layer_type = layers[layer]["name"].asString();

		if (layer_type == "COLLISION"){
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
		if(layer_type == "LIGHT"){
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

				glm::vec2 map_pos = glm::vec2(
					tiles[i]["x"].asInt(),
					tiles[i]["y"].asInt()
				);

				glm::vec2 mid_point = glm::vec2(
						position.x + (render_size.x/2),
						position.y + (render_size.y/2)
						);

				Light tmp_light;
				std::string light_id = std::format("{},{}",tiles[i]["x"].asString(), tiles[i]["y"].asString());
				std::cout << light_id << std::endl;


				if(this->light_map.find(light_id) != this->light_map.end()){
					tmp_light = this->light_map[light_id];
				}
				else{
					tmp_light = this->default_light;
				}

				tmp_light.pos = mid_point;
				tmp_light.map_pos = map_pos;
				this->lights->push_back(tmp_light);

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
	this->lights = map_config.lights;
	this->quads = map_config.quads;
	this->light_file= map_config.light_file;

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

	Json::CharReaderBuilder builder_light;
	std::ifstream ifs_light;

	ifs_light.open(this->light_file);
	if(!parseFromStream(builder_light, ifs_light, &this->light_root, &errs)){
		std::cout << "UNABLE TO OPEN LIGHT FILE resorting to default lights" << std::endl;
		this->has_light_file = false;
	}

	this->map_texture = new Texture(this->map_tileset, this->sprite_filter);
	this->pre_calc_collisions();

}

Map::~Map() {
	// Freeing shit ig
	this->map_texture->clear();
}

void Map::render() {
	// Clearing the quads before rendering
	auto layers = this->root["layers"];

	this->map_texture->bind();

	for (Json::Value::ArrayIndex layer = (layers.size() -1); layer != -1; layer--) {
		auto tiles = layers[layer]["tiles"];
		if(layers[layer]["name"].asString() != "COLLISION" && layers[layer]["name"].asString() != "LIGHT")
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

				this->quads->push_back(Quad {
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

