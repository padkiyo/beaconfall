#pragma once

#include "common.h"
#include "shader/shader.h"
#include "texture/texture.h"

struct FrameBuffer {
	u32 id;
	Texture* color_texture;
};

Result<FrameBuffer, const char*> fb_create(u32 width, u32 height);
void fb_destroy(FrameBuffer fb);
void fb_bind(FrameBuffer fb);
void fb_unbind(FrameBuffer fb);
