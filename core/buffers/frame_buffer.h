#pragma once

#include "common.h"
#include "shader/shader.h"
#include "texture/texture.h"

// NOTE(slok): This framebuffer only supports color channel

class FrameBuffer {
public:
	FrameBuffer(u32 width, u32 height);
	~FrameBuffer();

	void bind();
	void unbind();
	void bind_color_channel();

public:
	const Texture& get_color_texture() const { return *m_color_texture; }

private:
	u32 m_id;
	Texture* m_color_texture;
};

/*
struct FrameBuffer {
	u32 id;
	Texture* color_texture;
};

Result<FrameBuffer, const char*> fb_create(u32 width, u32 height);
void fb_destroy(FrameBuffer fb);
void fb_bind(FrameBuffer fb);
void fb_unbind(FrameBuffer fb);
*/
