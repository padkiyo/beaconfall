#include "animation_system.h"


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
