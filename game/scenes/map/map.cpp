#include "map.h"
#include "game_state.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
// ! TODO clean up ts
// ! TODO forsome reason complex maps dont load, need to find a fix for it
extern GameState gs;

// ! TODO maybe not make this global ig
Json::Value root;
Texture tileset;
// ! TODO get a better name for this function
glm::vec4 get_tex_cords_from_id(f32 id, i32 width, i32 height){
	bool done = false;
	u32 count = -1;

	while (!done) {
		count += 1;
		if (id < width) {
			done = true;
			break;
		}
		id -= width;
	}

	// TODO fix this f32 bullshit
	return glm::vec4((f32)id/width, (f32)count/height, 1/(f32)width , (1/(f32)height) );
}
void map_entry(void* data) {

	std::ifstream ifs;

	ifs.open("./game/maps/items_massive_test/map.json");

	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;

	JSONCPP_STRING errs;

	if(!parseFromStream(builder, ifs, &root, &errs)){
		std::cout << errs << std::endl;
	}

	tileset = texture_create_from_file("./game/maps/items_massive_test/spritesheet.png").unwrap();
	texture_bind(tileset);

}

void map_exit(void*data) {
	log_info("Map scene is exited\n");
}

void map_update(void*data, f64 dt) {

	rp_push_text(gs.quad_rp, gs.font_regular, "Map Scene", glm::vec3(0, 0, 0),
			glm::vec4(1,1,1,1));

	auto layers = root["layers"];
	f32 tile_size = root["tileSize"].asFloat();

	texture_bind(tileset);
	for (Json::Value::ArrayIndex i = (layers.size() -1); i != -1; i--)
	{
		auto tiles = layers[i]["tiles"];

		for(Json::Value::ArrayIndex i = 0; i != tiles.size(); i++)
		{
			glm::vec3 position = glm::vec3(
					tiles[i]["x"].asFloat() * tile_size * 3,
					tiles[i]["y"].asFloat() * tile_size * 3,
					1.0f
					);

			glm::vec2 size = glm::vec2(
					tile_size * 3,
					tile_size * 3
					);
			glm::vec4 texcoords = get_tex_cords_from_id(
					std::stof(tiles[i]["id"].asString()),
					tileset.width / tile_size,
					tileset.height / tile_size
					);

		rp_push_quad(gs.quad_rp, position, size, glm::vec4(1, 1, 1, 1), tileset.id, texcoords);

		}
	}
}

void map_event(void* data, SDL_Event event, f64 dt) {
}
