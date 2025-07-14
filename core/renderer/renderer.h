#pragma once

#include "common.h"

#include "shader/shader.h"
#include "texture/texture.h"
#include "font/font.h"
#include "camera/camera.h"
#include "buffers/vertex_buffer.h"
#include "buffers/vertex_array.h"
#include "buffers/frame_buffer.h"
#include "geometry.h"

// TODO(slok): Add support for auto flushing for vertex overflow

class Renderer {
public:
	Renderer();
	~Renderer();

	void start();
	void commit(const glm::vec2& res);

	void begin_pass(const glm::vec2& pos, const glm::vec2& res);
	void end_pass();

	// Render Commands
	void clear(const glm::vec4& color);
	void push_quad(const Quad& quad);
	void push_vertex(const Vertex& v);

public:
	const Texture& white_texture() const { return *m_white_texture; }

private:
	void execute_draw_call();

private:
	glm::mat4 m_view_proj;
	glm::vec2 m_res;

	// Buffers
	VertexBuffer* m_vbo;
	VertexArray* m_vao;

	// CPU Buffers
	std::vector<f32> m_buffer;

	// Framebuffers
	std::vector<FrameBuffer*> m_passes;

	// This texture is used by default if no texture is given
	Texture* m_white_texture;

	// Shaders
	Shader* m_shader;
};
