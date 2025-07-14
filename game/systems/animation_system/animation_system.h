#pragma once

#include "core.h"
#include "systems/sprite_system/sprite_system.h"

struct AnimationEntry {
	FrameId id;

	b32 loop;

	i32 curr_frame;
	u64 duration;

};


class Animator {
	public:
		SpriteId sprite_id;

		FrameId curr_state;
		u64 start_time;

		Animator(SpriteId sprite_id, FrameId frame_id);
		~Animator();

		std::unordered_map<FrameId, AnimationEntry> animations;

		void add_animation(FrameId id, u64 duration, b32 loop);

		glm::vec4 get_frame(SpriteManager& sm);

		void switch_frame(FrameId id);

};
