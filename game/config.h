#pragma once

#include "core.h"

#define VERSION "v0.0.0"
#define WIN_TITLE "Game6 " VERSION
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define MAX_TEXTURE_SAMPLES 32


static RenderPipelineSpecs QuadRendererSpecs = {
	.format = {
		{ .type = GL_FLOAT, .count = 3 }, // Position
		{ .type = GL_FLOAT, .count = 4 }, // Color
		{ .type = GL_FLOAT, .count = 2 }, // UV
		{ .type = GL_FLOAT, .count = 1 }, // Texture ID
	},
	.max_vertices = 10000,
	.shaders = {
		.vertex_shader = "./shaders/vertex.vert",
		.fragment_shader = "./shaders/fragment.frag"
	},
};

static void init_texture_samples(RenderPipeline* rp) {
	i32 samplers[MAX_TEXTURE_SAMPLES];
	for (i32 i = 0; i < MAX_TEXTURE_SAMPLES; i++)
		samplers[i] = i;

	// Providing samplers to the shader
	glc(glUseProgram(rp->shader));
	i32 loc = glc(glGetUniformLocation(rp->shader, "textures"));
	panic(loc != -1, "Cannot find uniform: textures\n");
	glc(glUniform1iv(loc, MAX_TEXTURE_SAMPLES, samplers));
}
