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
	// auto result = texture_create_from_file(tilesheet);

	map.tilesheet = new Texture(
		std::string(tilesheet), {
			.min_filter = GL_NEAREST,
			.mag_filter = GL_NEAREST,
			.wrap_s = GL_CLAMP_TO_EDGE,
			.wrap_t = GL_CLAMP_TO_EDGE,
			.flip = false,
		}
	);

	return map;
}

void map_render(RenderPipeline* quad_rp, Map* map, f32 size){
	auto layers = map->root["layers"];
	f32 tile_size = map->root["tileSize"].asFloat();

	map->tilesheet->bind();

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
				(11.0f - tiles[i]["y"].asFloat()) * render_size.y ,
					1.0f
					);


			// converting id to texture coords
			glm::vec4 texcoords = map_get_texcoords(

					// scary shit here
					std::stof(tiles[i]["id"].asString()),
					map->tilesheet->get_width() / tile_size,
					map->tilesheet->get_height() / tile_size
					);

			rp_push_quad(quad_rp, position, render_size, glm::vec4(1, 1, 1, 1), map->tilesheet->get_id(), texcoords);

		}
	}

}

MapManager* mm_create() {
	MapManager* mm = new MapManager;
	mm->current_map = -1;
	return mm;
}

void mm_destroy(MapManager* mm) {
	for (auto& [id, map] : mm->maps) {
		delete map;
	}
	delete mm;
}

Result<i32 , const char*> mm_add_map(
		MapManager* mm, i32 id,
		const char* json,
		const char* tilesheet
)
{
	Map* nm = new Map;
	Map _tmp_map = xx(map_load(json, tilesheet));

	nm->root = _tmp_map.root;
	nm->tilesheet = _tmp_map.tilesheet;

	mm->maps.insert_or_assign(id, nm);

	if (mm->current_map == -1)
		mm->current_map = id;

	return id;
}

void mm_switch_map(MapManager* mm, i32 id) {
	panic(mm->maps.find(id) != mm->maps.end(), "Map with ID: %d is not found", id);
	if (id == mm->current_map) return;
	mm->current_map= id;
}

void mm_remove_map(MapManager* mm, i32 id) {
	panic(mm->maps.find(id) != mm->maps.end(), "Map with ID: %d is not found", id);

	panic(mm->current_map == id, "Map with ID: %d is current map, unable to delete", id);

	Map* map = mm->maps[id];
	delete map;
	mm->maps.erase(id);
}

void mm_render_current(RenderPipeline* quad_rp, MapManager* mm, f32 size) {
	Map* map = mm->maps[mm->current_map];
	map_render(quad_rp, map, size);
}

