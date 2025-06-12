#include "frame_buffer.h"

Result<FrameBuffer, const char*> fb_create(u32 width, u32 height) {
	FrameBuffer fb;
	GLC(glGenFramebuffers(1, &fb.id));
	GLC(glBindFramebuffer(GL_FRAMEBUFFER, fb.id));

	fb.color_texture = texture_create_from_data(width, height, NULL, GL_RGB, GL_RGB);
	GLC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.color_texture.id, 0));

	// Setting up the attachments
	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
	GLC(glDrawBuffers(1, attachments));

	// Checking if the fbo is properly built
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return "Failed to create frame buffer.";

	// Unbinding the buffer
	texture_unbind(fb.color_texture);
	GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return fb;
}

void fb_destroy(FrameBuffer fb) {
	texture_destroy(fb.color_texture);
	GLC(glDeleteFramebuffers(1, &fb.id));
}

void fb_bind(FrameBuffer fb) {
	texture_bind(fb.color_texture);
	GLC(glBindFramebuffer(GL_FRAMEBUFFER, fb.id));
}

void fb_unbind(FrameBuffer fb) {
	GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
