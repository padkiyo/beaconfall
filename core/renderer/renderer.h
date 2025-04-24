#pragma once

#include "common.h"

#include "error/error.h"
#include "shader/shader.h"
#include "texture/texture.h"

// Calling array of floats as vertices
typedef std::vector<f32> Vertices;

// Sepcifications for shader
struct ShaderSpec {
	const char* vertex_shader;
	const char* fragment_shader;
};

// Sepcifications for vertex format
struct FormatSpec {
	u32 type;
	i32 count;
};

// Sepcifications for render pipeline
struct RenderPipelineSpecs {
	std::vector<FormatSpec> format;
	i32 max_vertices;
	ShaderSpec shaders;
};

struct RenderPipeline {
	u32 vbo;
	u32 vao;
	u32 shader;

	i32 max_vertices;
	i32 vertex_size; // No of items in a vertex

	f32* buffer;
	u32 buffer_size;
	u32 buffer_index;

	Texture white_texture;
};

Result<RenderPipeline, std::string> rp_create(RenderPipelineSpecs* specs);
void rp_destroy(RenderPipeline* rp);

void rp_begin(RenderPipeline* rp);
void rp_end(RenderPipeline* rp);
void rp_push_vertices(RenderPipeline* rp, const Vertices& vertices);

Vertices rp_create_quad(glm::vec3 pos, glm::vec2 size, glm::vec4 color);
Vertices rp_create_quad(glm::vec3 pos, glm::vec2 size, glm::vec4 color, u32 tex_id, glm::vec4 tex_coord);

// Utils
size_t sizeof_gl_type(GLenum gl_type);

