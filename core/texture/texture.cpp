#include "texture.h"

Result<Texture, std::string> texture_create_from_file(const char* filepath, b32 flip) {
	stbi_set_flip_vertically_on_load(flip);

	i32 w, h, c;
	u8* data = stbi_load(filepath, &w, &h, &c, 0);
	if (data == NULL) {
		std::stringstream ss;
		ss << "Failed to load file: " << filepath;
		return ss.str();
	}

	// Binding the texture
	u32 id;
	glc(glGenTextures(1, &id));
	glc(glBindTexture(GL_TEXTURE_2D, id));

	// Setting up some basic modes to display texture
	glc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	glc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	glc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Sending the pixel data to opengl
	GLenum internal_format, format;
	if (c == 1) {
		internal_format = GL_RED;
		format = GL_RED;
	} else if (c == 3) {
		internal_format = GL_RGB;
		format = GL_RGB;
	} else if (c == 4) {
		internal_format = GL_RGBA;
		format = GL_RGBA;
	}

	glc(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, GL_UNSIGNED_BYTE, data));
	glc(glBindTexture(GL_TEXTURE_2D, 0));

	if (data) {
		stbi_image_free(data);
	}

	return (Texture) {
		.id = id,
		.width = w,
		.height = h,
		.internal_format = internal_format,
		.format = format
	};
}

Texture texture_create_from_data(i32 width, i32 height, u32* data, GLenum internal_format, GLenum format) {
	u32 id;
	glc(glGenTextures(1, &id));
	glc(glBindTexture(GL_TEXTURE_2D, id));

	// Setting up some basic modes to display texture
	glc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	glc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	glc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Sending the pixel data to opengl
	glc(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
	glc(glBindTexture(GL_TEXTURE_2D, 0));

	return (Texture) {
		.id = id,
		.width = width,
		.height = height,
		.internal_format = internal_format,
		.format = format
	};
}

void texture_destroy(Texture texture) {
	glc(glDeleteTextures(1, &texture.id));
}

void texture_clear(Texture texture) {
	u32 t = 0;
	glc(glClearTexImage(texture.id, 0, texture.format, GL_UNSIGNED_BYTE, &t));
}

void texture_update(Texture texture, i32 lod, i32 x_offset, i32 y_offset, u32 width, u32 height, void* data) {
	texture_bind(texture);
	glc(glTextureSubImage2D(
		texture.id, lod, x_offset, y_offset,
		width, height, texture.format,
		GL_UNSIGNED_BYTE, data
	));
}

void texture_bind(Texture texture) {
	// glc(glBindTextureUnit(texture.id, texture.id));
	glc(glActiveTexture(GL_TEXTURE0 + (i32) texture.id));
	glc(glBindTexture(GL_TEXTURE_2D, texture.id));
}

void texture_unbind(Texture texture) {
	// glc(glBindTextureUnit(texture.id, 0));
	glc(glActiveTexture(GL_TEXTURE0 + (i32) texture.id));
	glc(glBindTexture(GL_TEXTURE_2D, 0));
}
