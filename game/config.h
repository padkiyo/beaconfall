#pragma once

#include "core.h"

// Core
#define VERSION "v0.0.0"
#define WIN_TITLE "Game6 " VERSION
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define FPS 60

// Controls
#define CAM_SPEED 3.0f

// Texture
// #define MAX_TEXTURE_SAMPLES 32

// Dialog
#define BUBBLE_PADDING 15
#define BUBBLE_Y_GAP 25
#define BUBBLE_OFFSET_FROM_CENTER 300
#define BUBBLE_START_Y 500
#define BUBBLE_SCROLL_SPEED 3.0f
#define BUBBLE_ACTIVE_ALPHA 1.0f
#define BUBBLE_INACTIVE_ALPHA 0.5f

// Notebook
#define NOTEBOOK_WIDTH 400
#define NOTEBOOK_HEIGHT 500
#define NOTEBOOK_PAD_X 10
#define NOTEBOOK_PAD_Y 10

/*
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
	rp->shader->bind();
	rp->shader->set_arrayi("textures", samplers, MAX_TEXTURE_SAMPLES);
}
*/
