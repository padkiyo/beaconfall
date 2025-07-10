#pragma once

#include "common.h"

#include "shader/shader.h"
#include "texture/texture.h"
#include "font/font.h"
#include "camera/camera.h"
#include "buffers/vertex_buffer.h"
#include "buffers/vertex_array.h"
#include "buffers/frame_buffer.h"

#define MAX_VERTEX_COUNT 10000
#define MAX_TEXTURE_SAMPLES 32
#define MAX_LIGHTS 32

struct Vertex {
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec2 uv;
	f32 tex_id;
};

struct Light {
	glm::vec2 pos;
	f32 radius;
	f32 intensity;
	f32 dir;
	f32 fov;
	glm::vec4 color;
};

// TODO(slok): Add Index Buffer support

class Renderer {
public:
	Renderer(const glm::vec2& resolution);
	~Renderer();

	void begin(Camera& camera);
	void end();

	// Render Commands
	void set_clear_color(const glm::vec4& color);
	void push_quad(
		const glm::vec3& pos, const glm::vec2& size, const glm::mat4& rot,
		const Texture& texture, const glm::vec4& uv, const glm::vec4& color
	);
	void push_vertex(const Vertex& v);
	void push_light(const Light& l);

	// Light configs
	void set_ambient_color(const glm::vec3& color);
	void set_light_pixel_size(const glm::vec2& size);

public:
	const Texture& white_texture() const { return *m_white_texture; }
	glm::vec2 m_res;

private:
	void execute_draw_call();
	void execute_color_pass();
	void execute_light_pass();

private:
	glm::mat4 m_view_proj;
	glm::vec4 m_clear_color = { 0, 0, 0, 1 };
	glm::vec3 m_ambient_color = { 0.1, 0.1, 0.1 };
	glm::vec2 m_light_pixel_size = glm::vec2(2);

	// Buffers
	VertexBuffer* m_vbo;
	VertexArray* m_vao;

	// CPU Buffers
	std::vector<f32> m_buffer;
	std::vector<Light> m_lights;

	// Framebuffers
	FrameBuffer* m_color_pass;
	FrameBuffer* m_light_pass;

	// This texture is used by default if no texture is given
	Texture* m_white_texture;

	// Shaders
	Shader* m_color_shader;
	Shader* m_light_shader;
	Shader* m_final_shader;
};
