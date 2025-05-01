#include "map.h"
#include "game_state.h"

extern GameState gs;

// ! TODO maybe not make this global ig
Json::Value root;
Texture tileset;
// ! TODO get a better name for this function
glm::vec4 get_tex_cords_from_id(f32 id, i32 width, i32 height){
	bool done = false;
	u32 count = 0;

	while (!done) {
		if (id < width) {
			done = true;
			break;
		}
		id -= width;
		count += 1;
	}
	return glm::vec4(id/width, (count/width), 1/(f32)width , (1/(f32)height) );
}
void map_entry(void* data) {

	std::ifstream ifs;

	ifs.open("./game/maps/test/map.json");

	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;

	JSONCPP_STRING errs;

	if(!parseFromStream(builder, ifs, &root, &errs)){
		std::cout << errs << std::endl;
	}

	tileset = texture_create_from_file("./game/maps/test/spritesheet.png").unwrap();
	texture_bind(tileset);

}

void map_exit(void*data) {
	log_info("Map scene is exited\n");
}

void map_update(void*data, f64 dt) {

	rp_push_text(gs.quad_rp, gs.font_regular, "Map Scene", glm::vec3(400, 0, 0),
			glm::vec4(1,1,1,1));

	auto layers = root["layers"];
	f32 tile_size = root["tileSize"].asFloat();

	texture_bind(tileset);
	for (Json::Value::ArrayIndex i = 0; i != layers.size(); i++)
	{
		auto tiles = layers[i]["tiles"];
		std::cout << layers[i]["name"] << std::endl;

		for(Json::Value::ArrayIndex i = 0; i != tiles.size(); i++)
		{
			glm::vec3 position = glm::vec3(
					tiles[i]["x"].asFloat() * tile_size,
					tiles[i]["y"].asFloat() * tile_size,
					1.0f
					);

			glm::vec2 size = glm::vec2(
					tile_size,
					tile_size
					);
			glm::vec4 texcoords = get_tex_cords_from_id(
					std::stof(tiles[i]["id"].asString()),
					tileset.width / tile_size,
					tileset.height / tile_size
					);

			// TODO fix this jumbled mess of  a shit
//			glm::vec4 texcoords = glm::vec4(
//					std::stof(tiles[i]["id"].asString())/4.0f,
//					0.0f,
//					0.25f,
//					1.0f
//			);

			std::cout << texcoords.x << " " << texcoords.y << " " << texcoords.z << " " << texcoords.w << std::endl;


		rp_push_quad(gs.quad_rp, position, size, glm::vec4(1, 1, 1, 1), tileset.id, texcoords);
		}
	}
}

void map_event(void* data, SDL_Event event, f64 dt) {
}
