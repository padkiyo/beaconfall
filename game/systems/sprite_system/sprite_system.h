#pragma once

#include "core.h"
// TODO add ts in config.h
#define MAX_SPRITE 40

enum SpriteId {
	PLAYER
};

enum FrameId {
	PLAYER_IDLE,
	PLAYER_DIE
};

struct Sprite {
	SpriteId id;
	const char* path;

	i32 x_cnt;
	i32 y_cnt;
};

class SpriteManager {
	public:
		SpriteManager();
		~SpriteManager();

		void add_sprite(Sprite sprite, u32 id);

		void load_sprites();

		std::unordered_map<SpriteId, Sprite> sprites;
		std::unordered_map<SpriteId, Texture*> sprite_textures;
		std::unordered_map<FrameId, std::vector<glm::vec4>> frames;

		void add_sprite(Sprite sprite, SpriteId sprite_id);
		void create_frame(SpriteId sprite_id, u32 row, u32 col, FrameId frame_id);

		void activate_spritesheet(SpriteId sprite_id);
		std::vector<glm::vec4> get_frame(FrameId frame_id);
		const Texture& get_spritesheet_texture(SpriteId sprite_id);

	private:
		TextureFilter sprite_filter = {
			.min_filter = GL_NEAREST,
			.mag_filter = GL_NEAREST,
			.wrap_s = GL_CLAMP_TO_EDGE,
			.wrap_t = GL_CLAMP_TO_EDGE,
			.flip = true
		};
};

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
