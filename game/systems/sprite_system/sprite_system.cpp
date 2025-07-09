#include "sprite_system.h"
#include "config.h"

SpriteManager::SpriteManager() {
}

SpriteManager::~SpriteManager() {
}

void SpriteManager::add_sprite(Sprite sprite, SpriteId sprite_id) {

	// Inserting the given sprite into hashmap
	this->sprites.insert({sprite_id, sprite});
	this->sprite_textures.insert({sprite_id, new Texture(sprite.path, this->sprite_filter)});
}

void SpriteManager::create_frame(SpriteId sprite_id, u32 row, u32 col, FrameId frame_id){

	// Checking if the sprite actuall exists
	if( this->sprites.find(sprite_id) != this->sprites.end() ){
		auto sprite = this->sprites[sprite_id];

		this->frames.insert({frame_id, {} });

		// Calculating texture corrds
		for (i32 i = 0; i <= col; i++) {

			f32 x = (f32)i / sprite.x_cnt;
			f32 y = (f32) row / sprite.y_cnt;
			f32 z = 1.0f / sprite.x_cnt;
			f32 w = 1.0f / sprite.y_cnt;

			// Pushing into each frame
			this->frames[frame_id].push_back(glm::vec4(x , y, z, w));
		}

	}
}

std::vector<glm::vec4> SpriteManager::get_frame(FrameId frame_id){

	if(this->frames.find(frame_id) != this->frames.end()){
		return this->frames[frame_id];
	}
	else return {};

}

void SpriteManager::activate_spritesheet(SpriteId sprite_id) {
	if(this->sprite_textures.find(sprite_id) != this->sprite_textures.end()){
		this->sprite_textures[sprite_id]->bind();
	}
}

const Texture& SpriteManager::get_spritesheet_texture(SpriteId sprite_id) {
	return *this->sprite_textures[sprite_id];
}


