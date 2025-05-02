#pragma once

#include "common.h"
#include "texture/texture.h"

struct Font {
	i32 size;
	TTF_Font* ttf_font;
	Texture atlas;
	std::unordered_map<char, std::pair<glm::vec4, glm::vec2>> glyphs;
};

Result<Font, std::string> font_create(const std::string& path, i32 size);
void font_destroy(Font* font);
void font_bind(Font* font);
glm::vec2 font_calc_size(Font* font, const std::string& text);
