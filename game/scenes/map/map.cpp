#include "map.h"
#include "game_state.h"

extern GameState gs;

void map_entry(void* data){

	Json::Value root;
	std::ifstream ifs;
	ifs.open("./game/maps/test/map.json");
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	JSONCPP_STRING errs;

	if(!parseFromStream(builder, ifs, &root, &errs)){
		std::cout << errs << std::endl;
	}

	std::cout << root["tileSize"] << std::endl;
}
void map_exit(void*data){
	log_info("Map scene is exited\n");
}
void map_update(void*data, f64 dt){

}
void map_event(void* data, SDL_Event event, f64 dt){}
