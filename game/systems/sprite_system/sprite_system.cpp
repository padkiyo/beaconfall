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


Animator::Animator(SpriteId sprite_id, FrameId frame_id) {
	this->sprite_id = sprite_id;
	this->curr_state = frame_id;
	this->start_time = SDL_GetTicks();
}

void Animator::add_animation(FrameId id, u64 duration, b32 loop) {
	this->animations.insert({id, {
				.id = id,
				.loop = loop,
				.duration = duration
			}
		});
}

glm::vec4 Animator::get_frame(SpriteManager& sm) {
	auto anim_entry = this->animations[this->curr_state];

	u64 dt = (SDL_GetTicks() - this->start_time);

	if(dt >= anim_entry.duration && anim_entry.loop) {
		this->start_time = SDL_GetTicks();
		dt = (SDL_GetTicks() - this->start_time);
	}

	u32 frame_count = sm.get_frame(this->curr_state).size();
	f64 fpt = frame_count  /(f64) anim_entry.duration;
	this->animations[this->curr_state].curr_frame = floor(fpt * dt);
	i32 current_frame = floor(fpt*dt);

	if (current_frame >= frame_count) {
		current_frame = frame_count - 1;
	}

	//std::cout << current_frame << std::endl;
	return sm.get_frame(this->curr_state)[current_frame];
}

void Animator::switch_frame(FrameId id) {
	if (this->curr_state == id) {
		return;
	}

	this->curr_state = id;
	this->start_time = SDL_GetTicks();

	this->animations[this->curr_state].curr_frame = 0;
}
