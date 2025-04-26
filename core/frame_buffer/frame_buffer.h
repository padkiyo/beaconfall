#pragma once

#include "common.h"
#include "error/error.h"
#include "shader/shader.h"
#include "texture/texture.h"
#include "base/utils.h"

struct FrameBuffer {
	u32 id;
	Texture color_texture;
};

FrameBuffer fb_create(u32 width, u32 height);
void fb_bind(FrameBuffer* fb);
void fb_unbind(FrameBuffer* fb);
