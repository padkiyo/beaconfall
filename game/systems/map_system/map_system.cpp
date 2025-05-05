// TODO: better error handling by saying which file is not loading
//
#include "map_system.h"
#include "maps.h"

glm::vec4 map_get_texcoords(f32 id, f32  width, f32 height){
	bool done = false;
	f32 count = -1.0f;

	while (!done) {
		count += 1.0f;
		if (id < width) {
			done = true;
			break;
		}
		id -= width;
	}

	return glm::vec4(id/width, count/height, 1/width , 1/height);
}

Result <Map, const char*> map_load(const char* json, const char* tilesheet) {

	Map map;

	Texture _tilesheet;
	Json::CharReaderBuilder builder;
	std::ifstream ifs;
	JSONCPP_STRING errs;

	// Trying to open map file
	ifs.open(json);
	builder["collectComments"] = false;

	// Trying to parse it to json
	if(!parseFromStream(builder, ifs, &map.root, &errs)){
		return "Unable to parse map file";
	}

	// Trying to open map tileset
	auto result = texture_create_from_file(tilesheet);

	if (result.is_err())
	{
		return "Unable to parse map tilesheet";
	}
	else{
		_tilesheet = result.unwrap();
	}

	map.tilesheet = _tilesheet;
	return map;

}

void map_render(RenderPipeline* quad_rp, Map* map, f32 size){

	auto layers = map->root["layers"];
	f32 tile_size = map->root["tileSize"].asFloat();

	texture_bind(map->tilesheet);

	// Rendering layers
	for (Json::Value::ArrayIndex layer  = (layers.size() -1); layer != -1; layer--)
	{
		auto tiles = layers[layer]["tiles"];

		// Rendering tiles
		for(Json::Value::ArrayIndex i = 0; i != tiles.size(); i++)
		{

			glm::vec2 render_size = glm::vec2(
					tile_size * size,
					tile_size * size
					);

			glm::vec3 position = glm::vec3(
					tiles[i]["x"].asFloat() * render_size.x,
					tiles[i]["y"].asFloat() * render_size.y,
					1.0f
					);


			// converting id to texture coords
			glm::vec4 texcoords = map_get_texcoords(

					// scary shit here
					std::stof(tiles[i]["id"].asString()),
					map->tilesheet.width / tile_size,
					map->tilesheet.height / tile_size
					);

			rp_push_quad(quad_rp, position, render_size, glm::vec4(1, 1, 1, 1), map->tilesheet.id, texcoords);

		}
	}

}
