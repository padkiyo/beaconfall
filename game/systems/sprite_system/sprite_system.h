#pragma once

#include "core.h"

enum SpriteId {
	PLAYER,
	ROCK,
	GEM,
	BEACON
};

enum FrameId {
	PLAYER_IDLE,
	PLAYER_DIE,
	BEACON_DEFAULT
};

struct Sprite {
	SpriteId id;
	const char* path;
	i32 x_cnt;
	i32 y_cnt;
};

class SpriteManager {
	public:
		std::unordered_map<SpriteId, Sprite> sprites;
		std::unordered_map<SpriteId, Texture*> sprite_textures;
		std::unordered_map<FrameId, std::vector<glm::vec4>> frames;

	public:
		SpriteManager();
		~SpriteManager();

		void add_sprite(Sprite sprite, SpriteId sprite_id);
		void create_frame(SpriteId sprite_id, u32 row, u32 col, FrameId frame_id);
		void activate_spritesheet(SpriteId sprite_id);
		const std::vector<glm::vec4>& get_frame(FrameId frame_id) const;
		const Texture& get_spritesheet_texture(SpriteId sprite_id) const;

	private:
		TextureFilter sprite_filter = {
			.min_filter = GL_NEAREST,
			.mag_filter = GL_NEAREST,
			.wrap_s = GL_CLAMP_TO_EDGE,
			.wrap_t = GL_CLAMP_TO_EDGE,
			.flip = true
		};
};


