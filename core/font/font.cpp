#include "font.h"

// NOTE: We are only going to support these characters
// If some of them are missing add here
std::string default_char_set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?\"'()[]{}:;<>+-=*/\\@#$%^&*_~ ";


std::unordered_map<char, SDL_Surface*> load_char_set(TTF_Font* ttf_font, i32* width, i32* height) {
	std::unordered_map<char, SDL_Surface*> surfs;

	for (char x : default_char_set) {
		char xs[2];
		xs[0] = x;
		xs[1] = '\0';

		SDL_Color color = { 255, 255, 255, 255 };
		SDL_Color bg = { 0, 0, 0, 0};
		SDL_Surface* surface = TTF_RenderText_Shaded(ttf_font, xs, color, bg);
		panic(surface, "Failed to create surface");

		// Converting the surface to RGBA format
		if (surface->format->format != SDL_PIXELFORMAT_RGBA8888) {
			SDL_Surface* formatted_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
			panic(formatted_surface, "Error converting surface format");

			SDL_FreeSurface(surface);
			surface = formatted_surface;
		}

		// The atlas is a 1D wide texture
		*width += surface->w;
		if (*height < surface->h) {
			*height = surface->h;
		}

		surfs.insert({ x, surface });
	}

	return surfs;
}

Result<Font, std::string> font_create(const std::string& path, i32 size) {
	if (TTF_Init() < 0) {
		return std::string("Failed to initialize SDL_ttf");
	}

	// Open the font
	TTF_Font* ttf_font = TTF_OpenFont(path.c_str(), size);
	if (ttf_font == NULL) {
		return std::format("Cannot open font: {}", path);
	}

	// Generate atlas dimensions and load up the surfaces for character set
	i32 atlas_width = 0;
	i32 atlas_height = 0;
	std::unordered_map<char, SDL_Surface*> surfs = load_char_set(ttf_font, &atlas_width, &atlas_height);

	// Creating an atlas and a glyph table
	TextureFilter filter = {
		.min_filter = GL_LINEAR,
		.mag_filter = GL_LINEAR,
		.wrap_s = GL_CLAMP_TO_EDGE,
		.wrap_t = GL_CLAMP_TO_EDGE
	};

	Texture atlas = texture_create_from_data(atlas_width, atlas_height, NULL, GL_RGBA8, GL_RGBA, filter);

	std::unordered_map<char, std::pair<glm::vec4, glm::vec2>> glyphs;

	// Constructing the atlas and saving the texture coordinates in glyph table
	i32 x_offset = 0;
	for (auto& [c, surf] : surfs) {
		// Updating the texture atlas
		texture_update(
			atlas,
			0,
			x_offset,
			0,
			surf->w,
			surf->h,
			surf->pixels
		);

		// Updating the glyph table
		glyphs.insert({ c, std::make_pair(
			glm::vec4((f32) x_offset / atlas_width, 0, (f32) surf->w / atlas_width, 1),
			glm::vec2(surf->w, surf->h)
		)});

		// Increasing the offset
		x_offset += surf->w;

		SDL_FreeSurface(surf);
	}

	texture_bind(atlas);

	return (Font) {
		.size = size,
		.ttf_font = ttf_font,
		.atlas = atlas,
		.glyphs = glyphs
	};
}

void font_destroy(Font* font) {
	TTF_CloseFont(font->ttf_font);
	texture_destroy(font->atlas);
	font->glyphs.clear();
}

void font_bind(Font* font) {
	texture_bind(font->atlas);
}
