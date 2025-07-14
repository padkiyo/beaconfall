#pragma once

#include "common.h"
#include "texture/texture.h"

struct Glyph {
	glm::vec4 uv;
	glm::vec2 size;
};

class Font {
public:
	Font(const std::string& path, i32 size);
	~Font();

	void bind();
	glm::vec2 calculate_dim(const std::string& text);
	glm::vec2 pixel_size();
	Glyph get_glyph(char c);
	inline const Texture& get_atlas() const { return *m_atlas; }

private:
	std::unordered_map<char, SDL_Surface*> load_char_set();

private:
	glm::vec2 m_size = {-1,-1};
	TTF_Font* m_font;
	glm::vec2 m_atlas_dim = {0,0};
	Texture* m_atlas;
	std::unordered_map<char, Glyph> m_glyphs;

	// NOTE: We are only going to support these characters
	// If some of them are missing add here
	std::string m_default_char_set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?\"'()[]{}:;<>+-=*/\\@#$%^&*_~ ";
};
