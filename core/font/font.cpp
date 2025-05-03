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
		SDL_Color bg = { 0, 0, 0, 0 };
		SDL_Surface* surface = TTF_RenderText_Blended(ttf_font, xs, color);
		panic(surface, "Failed to create surface");

		// Blitting the surface to an intermediate RGBA8 surface for proper formatting
		SDL_Surface *intermediate_surface = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, 32, SDL_PIXELFORMAT_RGBA8888);
		SDL_BlitSurface(surface, NULL, intermediate_surface, NULL);
		SDL_FreeSurface(surface);  // Free the original surface

		// Switching the surface
		surface = intermediate_surface;

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

	Texture atlas = texture_create_from_data(
		atlas_width, atlas_height, NULL,
		GL_RGBA8, GL_RGBA, filter
	);

	std::unordered_map<char, std::pair<glm::vec4, glm::vec2>> glyphs;

	// Constructing the atlas and saving the texture coordinates in glyph table
	i32 x_offset = 0;
	for (auto& [c, surf] : surfs) {

		// NOTE: Uncomment these to print the format of the texture
		//printf("SDL Surface Rmask: 0x%08x\n", surf->format->Rmask);
		//printf("SDL Surface format: %s\n", SDL_GetPixelFormatName(surf->format->format));

		// Updating the texture atlas
		texture_update(
			atlas,
			0,
			x_offset,
			0,
			surf->w,
			surf->h,
			GL_RGBA,
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

glm::vec2 font_calc_size(Font* font, const std::string& text) {
	glm::vec2 final_size = { 0, 0 };
	f32 line_width = 0.0f;
	for (char c : text) {
		if (c == '\n') {
			final_size.y += font->atlas.height;
			line_width = 0;
			continue;
		}

		auto [uv, size] = font->glyphs[c];

		// Increasing the width
		line_width += size.x;

		// Setting up the height
		if (final_size.y < size.y) {
			final_size.y = size.y;
		}

		// Setting up the width
		if (line_width > final_size.x) {
			final_size.x = line_width;
		}
	}

	return final_size;
}
