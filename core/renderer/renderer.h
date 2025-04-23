#pragma once

#include "common.h"

#include "error/error.h"
#include "shader/shader.h"
#include "texture/texture.h"

struct ShaderSpec {
	const char* vertex_shader;
	const char* fragment_shader;
};

struct FormatSpec{
	u32 type;
	i32 count;
};

struct RenderPipelineSpecs {
	std::vector<FormatSpec> format;
	i32 max_vertices;
	ShaderSpec shaders;
};

struct RenderPipeline {
	u32 vbo;
	u32 vao;
	u32 shader;
	f32* buffer;
	u32 buffer_size;
	u32 buffer_index;
	u32 buffer_actual_size;
	u32 frame_buffer;
	Texture frame_texture;
};

Result<RenderPipeline, const char*>  rp_create(RenderPipelineSpecs* specs);

void rp_begin(RenderPipeline* rp);
void rp_push_vertex(RenderPipeline* rp,  std::vector<f32> vertices);
void rp_push_quad(RenderPipeline* rp, glm::vec2 pos, f32 size, glm::vec4 color);
void rp_push_quad_tex(RenderPipeline* rp, glm::vec2 pos, f32 size, std::vector<glm::vec2> texture_coords);
void rp_end(RenderPipeline* rp);


void rp_push_quad(RenderPipeline* rp, glm::vec2 pos, f32 size, glm::vec4 color, glm::mat4 rot);
void rp_push_quad(RenderPipeline* rp, glm::vec2 pos, f32 size, u32 texture_id, glm::vec4 tex_cord, glm::mat4 rot);

