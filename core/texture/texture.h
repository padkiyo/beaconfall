#pragma once

#include "common.h"

typedef struct {
	u32 id;
	i32 width, height;
	GLenum internal_format, format;
} Texture;

/*
 * NOTE: The filter parameter for the textures are hard coded for now.
 * Introduce custom filter handlers when needed
 */

Result<Texture, std::string> texture_create_from_file(const char* filepath, b32 flip = false);

/*
 * NOTE: The 'data' parameter is always of type GL_UNSIGNED_BYTE thats why its a (u32*)
 * If texture of different data type is ever needed maybe we need to convert this function to a template
 */
Texture texture_create_from_data(i32 width, i32 height, u32* data, GLenum internal_format = GL_RGBA8, GLenum format = GL_RGBA);

void texture_destroy(Texture texture);

// Memsets the texture data to 0 in GPU
void texture_clear(Texture texture);

void texture_update(Texture texture, i32 lod, i32 x_offset, i32 y_offset, u32 width, u32 height, void* data);

/*
 * NOTE: Bindings are done using glActiveTexture with glBindTexture
 */
void texture_bind(Texture texture);
void texture_unbind(Texture texture);
