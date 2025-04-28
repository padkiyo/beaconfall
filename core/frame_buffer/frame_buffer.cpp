#include "frame_buffer.h"


FrameBuffer fb_create(u32 width, u32 height){
	FrameBuffer fb;
	glc(glGenFramebuffers(1, &fb.id));
	glc(glBindFramebuffer(GL_FRAMEBUFFER, fb.id));

	Texture frame_texture = texture_create_from_data(width, height, NULL, GL_RGB, GL_RGB);

	fb.color_texture = frame_texture;

	glc(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame_texture.id, 0));

	return fb;
}

void fb_bind(FrameBuffer* fb) {
	texture_bind(fb->color_texture);
	glc(glBindFramebuffer(GL_FRAMEBUFFER, fb->id));
}

void fb_unbind(FrameBuffer* fb) {
	glc(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
