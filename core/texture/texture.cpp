#include "texture.h"

Texture::Texture(const std::string& filepath, const TextureFilter& filter) {
	stbi_set_flip_vertically_on_load(filter.flip);

	i32 w, h, c;
	u8* data = stbi_load(filepath.c_str(), &w, &h, &c, 0);
	panic(data, "Failed to load Texture file: %s", filepath.c_str());

	m_width = static_cast<u32>(w);
	m_height = static_cast<u32>(h);

	// Binding the texture
	GLC(glGenTextures(1, &m_id));
	GLC(glBindTexture(GL_TEXTURE_2D, m_id));

	// Setting up some basic modes to display texture
	GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter.min_filter));
	GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter.mag_filter));
	GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, filter.wrap_s));
	GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, filter.wrap_t));

	// Setting up the format
	if (c == 1) {
		m_format.internal_format = GL_RED;
		m_format.format = GL_RED;
	} else if (c == 3) {
		m_format.internal_format = GL_RGB;
		m_format.format = GL_RGB;
	} else if (c == 4) {
		m_format.internal_format = GL_RGBA;
		m_format.format = GL_RGBA;
	}

	// Sending the pixel data to opengl
	GLC(glTexImage2D(GL_TEXTURE_2D, 0, m_format.internal_format, m_width, m_height, 0, m_format.format, GL_UNSIGNED_BYTE, data));
	GLC(glBindTexture(GL_TEXTURE_2D, 0));

	if (data) {
		stbi_image_free(data);
	}
}

Texture::Texture(u32 width, u32 height, void* data, const TextureFormat& format, const TextureFilter& filter)
	: m_width(width), m_height(height), m_format(format) {
	GLC(glGenTextures(1, &m_id));
	GLC(glBindTexture(GL_TEXTURE_2D, m_id));

	// Setting up some basic modes to display texture
	GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter.min_filter));
	GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter.mag_filter));
	GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, filter.wrap_s));
	GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, filter.wrap_t));

	// TODO(slok): Handle the flip filter

	// Sending the pixel data to opengl
	GLC(glTexImage2D(GL_TEXTURE_2D, 0, m_format.internal_format, m_width, m_height, 0, m_format.format, GL_UNSIGNED_BYTE, data));
	GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture() {
	GLC(glDeleteTextures(1, &m_id));
}

void Texture::bind() {
	GLC(glActiveTexture(GL_TEXTURE0 + (i32) m_id));
	GLC(glBindTexture(GL_TEXTURE_2D, m_id));
}

void Texture::unbind() {
	GLC(glActiveTexture(GL_TEXTURE0 + (i32) m_id));
	GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::update(i32 lod, i32 x_offset, i32 y_offset, u32 width, u32 height, void* data) {
	bind();
	GLC(glTextureSubImage2D(m_id, lod, x_offset, y_offset, width, height, m_format.format, GL_UNSIGNED_BYTE, data));
}

void Texture::clear() {
	bind();
	u32 t = 0;
	GLC(glClearTexImage(m_id, 0, m_format.format, GL_UNSIGNED_BYTE, &t));
}
