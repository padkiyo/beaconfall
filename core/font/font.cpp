#include "font.h"

Font::Font(const std::string& path, i32 size) {
	panic(TTF_Init() == 0, "Failed to initialize SDL2_ttf");

	m_font = TTF_OpenFont(path.c_str(), size);
	panic(m_font, "Failed to open font: %s\n", path.c_str());

	// Loading the surfaces of the character set
	std::unordered_map<char, SDL_Surface*> surfs = load_char_set();

	// Creating a texture atlas
	TextureFilter filter = {
		.min_filter = GL_LINEAR,
		.mag_filter = GL_LINEAR,
		.wrap_s = GL_CLAMP_TO_EDGE,
		.wrap_t = GL_CLAMP_TO_EDGE,
		.flip = false,
	};

	TextureFormat format = {
		.internal_format = GL_RGBA8,
		.format = GL_RGBA,
	};

	m_atlas = new Texture(m_atlas_dim.x, m_atlas_dim.y, NULL, format, filter);

	// Constructing the atlas and saving the texture coordinates in glyph table
	i32 x_offset = 0;
	for (auto& [c, surf] : surfs) {

		// NOTE: Uncomment these to print the format of the texture
		//printf("SDL Surface Rmask: 0x%08x\n", surf->format->Rmask);
		//printf("SDL Surface format: %s\n", SDL_GetPixelFormatName(surf->format->format));

		// Updating the texture atlas
		m_atlas->update(
			0,
			x_offset,
			0,
			surf->w,
			surf->h,
			surf->pixels
		);

		// Updating the glyph table
		// NOTE(slok): Here the width of uv is (1 / no_of_char)
		// It is assumed that each character is of same size
		m_glyphs.insert({ c, Glyph {
			.uv = {
				(f32) x_offset / m_atlas_dim.x, 0,
				1.0f / m_default_char_set.length(), 1
			},
			.size = { surf->w, surf->h }
		}});

		// Increasing the offset
		x_offset += surf->w;

		SDL_FreeSurface(surf);
	}
}

Font::~Font() {
	TTF_CloseFont(m_font);
	delete m_atlas;
}

std::unordered_map<char, SDL_Surface*> Font::load_char_set() {
	std::unordered_map<char, SDL_Surface*> surfs;

	for (char x : m_default_char_set) {
		char xs[2];
		xs[0] = x;
		xs[1] = '\0';

		SDL_Color color = { 255, 255, 255, 255 };
		SDL_Color bg = { 0, 0, 0, 0 };
		SDL_Surface* surface = TTF_RenderText_Blended(m_font, xs, color);
		panic(surface, "Failed to create surface");

		// Blitting the surface to an intermediate RGBA8 surface for proper formatting
		SDL_Surface *intermediate_surface = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, 32, SDL_PIXELFORMAT_RGBA8888);
		SDL_BlitSurface(surface, NULL, intermediate_surface, NULL);
		SDL_FreeSurface(surface);  // Free the original surface

		// Switching the surface
		surface = intermediate_surface;

		// NOTE: Flipping the surface because of perspective projection that we're using
		flip_surface(surface);

		// The atlas is a 1D wide texture
		m_atlas_dim.x += surface->w;
		if (m_atlas_dim.y < surface->h) {
			m_atlas_dim.y = surface->h;
		}

		surfs.insert({ x, surface });
	}

	return surfs;
}

void Font::bind() {
	m_atlas->bind();
}

glm::vec2 Font::calculate_dim(const std::string& text) {
	glm::vec2 final_size = { 0, 0 };
	f32 line_width = 0.0f;
	for (char c : text) {
		if (c == '\n') {
			final_size.y += m_atlas->get_height();
			line_width = 0;
			continue;
		}

		Glyph glyph = m_glyphs[c];
		glm::vec4 uv = glyph.uv;
		glm::vec2 size = glyph.size;

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

glm::vec2 Font::pixel_size() {
	if (m_size.x == -1 && m_size.y == -1) {
		m_size = calculate_dim(" ");
	}
	return m_size;
}

Glyph Font::get_glyph(char c) {
	panic(m_glyphs.find(c) != m_glyphs.end(), "Failed to find glyph for: %c\n", c);
	return m_glyphs[c];
}
