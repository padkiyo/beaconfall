#include "frame_buffer.h"

Result<FrameBuffer, const char*> fb_create(u32 width, u32 height) {
	FrameBuffer fb;
	GLC(glGenFramebuffers(1, &fb.id));
	GLC(glBindFramebuffer(GL_FRAMEBUFFER, fb.id));

	TextureFormat format = {
		.internal_format = GL_RGBA8,
		.format = GL_RGBA,
	};

	TextureFilter filter = {
		.min_filter = GL_NEAREST,
		.mag_filter = GL_NEAREST,
		.wrap_s = GL_CLAMP_TO_EDGE,
		.wrap_t = GL_CLAMP_TO_EDGE,
	};

	fb.color_texture = new Texture(width, height, NULL, format, filter);
	GLC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.color_texture->get_id(), 0));

	// Setting up the attachments
	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
	GLC(glDrawBuffers(1, attachments));

	// Checking if the fbo is properly built
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return "Failed to create frame buffer.";

	// Unbinding the buffer
	fb.color_texture->unbind();
	GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return fb;
}

void fb_destroy(FrameBuffer fb) {
	delete fb.color_texture;
	GLC(glDeleteFramebuffers(1, &fb.id));
}

void fb_bind(FrameBuffer fb) {
	fb.color_texture->bind();
	GLC(glBindFramebuffer(GL_FRAMEBUFFER, fb.id));
}

void fb_unbind(FrameBuffer fb) {
	fb.color_texture->unbind();
	GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
