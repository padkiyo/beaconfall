#pragma once

#include "common.h"

struct Texture {
	u32 id;
	i32 width, height;
	GLenum internal_format, format;
};

struct TextureFilter {
	GLenum min_filter;
	GLenum mag_filter;
	GLenum wrap_s;
	GLenum wrap_t;
};

static TextureFilter DefaultFilter = {
	.min_filter = GL_NEAREST,
	.mag_filter = GL_NEAREST,
	.wrap_s = GL_CLAMP_TO_EDGE,
	.wrap_t = GL_CLAMP_TO_EDGE
};

Result<Texture, std::string> texture_create_from_file(const char* filepath, b32 flip = false, TextureFilter filter = DefaultFilter);

/*
 * NOTE: The 'data' parameter is always of type GL_UNSIGNED_BYTE thats why its a (u32*)
 * If texture of different data type is ever needed maybe we need to convert this function to a template
 */
Texture texture_create_from_data(i32 width, i32 height, u32* data, GLenum internal_format = GL_RGBA8, GLenum format = GL_RGBA, TextureFilter filter = DefaultFilter);

void texture_destroy(Texture texture);

// Memsets the texture data to 0 in GPU
void texture_clear(Texture texture);

void texture_update(Texture texture, i32 lod, i32 x_offset, i32 y_offset, u32 width, u32 height, void* data);

/*
 * NOTE: Bindings are done using glActiveTexture with glBindTexture
 */
void texture_bind(Texture texture);
void texture_unbind(Texture texture);
