#pragma once

#include "common.h"

#include "shader/shader.h"
#include "texture/texture.h"
#include "font/font.h"
#include "camera/camera.h"
#include "buffers/vertex_buffer.h"
#include "buffers/vertex_array.h"

#define MAX_VERTEX_COUNT 1000
#define MAX_TEXTURE_SAMPLES 32

struct Vertex {
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec2 uv;
	f32 tex_id;
};

class Renderer {
public:
	Renderer();
	~Renderer();

	void begin(Camera& camera);
	void end();

	void push_quad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
	void push_vertex(const Vertex& v);

private:
	VertexBuffer* m_vbo;
	VertexArray* m_vao;

	// This texture is used by default if no texture is given
	Texture* m_white_texture;

	// Current Shader holds the shader that is used to render
	// Default Shader holds the default renderer shader
	// NOTE(slok): Introduced this to make sure we can switch shaders
	Shader* m_current_shader;
	Shader* m_default_shader;

	glm::mat4 m_view_proj;

	// CPU Vertex Buffer
	std::vector<f32> m_buffer;
};


/*
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
	Shader* shader;

	i32 max_vertices;
	i32 vertex_size; // No of items in a vertex

	f32* buffer;
	u32 buffer_size;
	u32 buffer_index;

	Texture* white_texture;
};

Result<RenderPipeline*, std::string> rp_create(RenderPipelineSpecs* specs);
void rp_destroy(RenderPipeline* rp);

void rp_begin(RenderPipeline* rp);
void rp_end(RenderPipeline* rp);
void rp_push_vertices(RenderPipeline* rp, const Vertices& vertices);

// Quads
Vertices rp_create_quad(glm::vec3 pos, glm::vec2 size, glm::vec4 color);
Vertices rp_create_quad(glm::vec3 pos, glm::vec2 size, glm::vec4 color, u32 tex_id, glm::vec4 tex_coord, glm::mat4 rot = glm::mat4(1));

#define rp_push_quad(rp, ...) do {\
	Vertices x = rp_create_quad(__VA_ARGS__);\
	rp_push_vertices((rp), x);\
} while (0)

// Text
Vertices rp_create_text(Font* font, const std::string& text, glm::vec3 pos, glm::vec4 color);

#define rp_push_text(rp, ...) do {\
	Vertices x = rp_create_text(__VA_ARGS__);\
	rp_push_vertices((rp), x);\
} while (0)
*/
