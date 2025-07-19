#include "renderer.h"

Renderer::Renderer() {
	// Constructing Vertex Array
	m_vao = new VertexArray();

	// Setting up the vertex layout
	m_vao->push_layout<f32>(3); // Position
	m_vao->push_layout<f32>(4); // Color
	m_vao->push_layout<f32>(2); // UV
	m_vao->push_layout<f32>(1); // Texture ID

	// Constructing Vertex Buffer
	ssize_t vbo_size = MAX_VERTEX_COUNT * m_vao->get_stride();
	m_vbo = new VertexBuffer(vbo_size, nullptr, GL_DYNAMIC_DRAW);

	// Constructing Index Buffer
	m_ibo = new IndexBuffer(MAX_INDEX_COUNT);
	m_ibo->generate_quad_indices();

	// NOTE(slok): Make sure to always build VAO only after binding VBO
	m_vao->build();

	// Allocate CPU buffer
	m_buffer.reserve(MAX_VERTEX_COUNT * m_vao->get_count());

	// Constructing Shader
	m_shader = new Shader("./shaders/plain.vert", "./shaders/color.frag", ShaderLoadType::FromFile);

	// Constructing a white texture
	u32 data = 0xffffffff;
	m_white_texture = new Texture(
		1, 1, &data,
		{
			.internal_format = GL_RGBA8,
			.format = GL_RGBA,
		},
		{
			.min_filter = GL_NEAREST,
			.mag_filter = GL_NEAREST,
			.wrap_s = GL_CLAMP_TO_EDGE,
			.wrap_t = GL_CLAMP_TO_EDGE,
			.flip = false,
		}
	);

	// Provide texture samplers
	i32 samplers[MAX_TEXTURE_SAMPLES];
	for (i32 i = 0; i < MAX_TEXTURE_SAMPLES; i++)
		samplers[i] = i;

	// Providing samplers to the shaders
	m_shader->bind();
	m_shader->set_arrayi("textures", samplers, MAX_TEXTURE_SAMPLES);

	// Enabling the Opengl Settings
	GLC(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLC(glEnable(GL_BLEND));
	GLC(glEnable(GL_DEPTH_TEST));
}

Renderer::~Renderer() {
	delete m_vbo;
	delete m_ibo;
	delete m_vao;
	delete m_white_texture;
	delete m_shader;

	for (auto pass : m_passes) {
		delete pass;
	}
}

void Renderer::start() {
	// NOTE(slok): Deleting framebuffers every frame and recreating one might be slow
	for (auto pass : m_passes) {
		delete pass;
	}

	m_white_texture->bind();
	m_passes.clear();

	// Clean the status
	m_stat.draw_calls = 0;
	m_stat.vertex_counts = 0;
}

void Renderer::commit(const glm::vec2& res) {
	GLC(glViewport(0, 0, res.x, res.y));

	// Clearing the buffer
	m_buffer.clear();

	// Binding the resources
	m_shader->bind();

	// Binding the last pass's texture
	FrameBuffer* last_pass = m_passes.back();
	last_pass->bind_color_channel();

	// Rendering the last pass's texture
	push_quad(Quad {
		{-1,-1,0},
		{2, 2},
		glm::mat4(1),
		&last_pass->get_color_texture(),
		{0,0,1,1},
		{1,1,1,1}
	});
	execute_draw_call();
}

void Renderer::begin_pass(const glm::vec2& pos, const glm::vec2& res) {
	m_res = res;
	m_buffer.clear();

	// Creating a new pass
	FrameBuffer* new_pass = new FrameBuffer(m_res.x, m_res.y);
	m_passes.push_back(new_pass);

	new_pass->bind();
	GLC(glViewport(0, 0, m_res.x, m_res.y));

	// Pushing the last pass's texture in the new pass
	if (m_passes.size() >= 2) {
		FrameBuffer* prev_pass = m_passes[m_passes.size() - 2];
		push_quad(Quad {
			{pos.x, pos.y, 0},
			m_res,
			glm::mat4(1),
			&prev_pass->get_color_texture(),
			{0,0,1,1},
			{1,1,1,1}
		});
	}
}

void Renderer::end_pass() {
	// Binding the prev pass frame buffer
	if (m_passes.size() >= 2) {
		FrameBuffer* prev_pass = m_passes[m_passes.size() - 2];
		prev_pass->bind_color_channel();
	}

	// Rendering
	execute_draw_call();

	// Unbinding the current pass
	FrameBuffer* curr_pass = m_passes.back();
	curr_pass->unbind();
}

void Renderer::execute_draw_call() {
	m_vbo->push_data(0, m_buffer.size() * sizeof(f32), (void*)m_buffer.data());
	m_ibo->bind();
	m_vao->bind();

	// Calculating amount of indices to be rendered
	u32 count = (m_buffer.size() / (m_vao->get_count() * 4)) * 6;

	GLC(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL));

	// Gathering stats
	m_stat.draw_calls++;
	m_stat.vertex_counts += m_buffer.size() / m_vao->get_count();
}

void Renderer::clear(const glm::vec4& color) {
	GLC(glClearColor(color.r, color.g, color.b, color.a));
	GLC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::push_quad(const Quad& quad) {
	if ((m_buffer.size() / m_vao->get_count()) + 1 >= MAX_QUAD_COUNT) {
		execute_draw_call();
		m_buffer.clear();
	}

	glm::vec3 pos = quad.pos;
	glm::vec2 size = quad.size;
	glm::mat4 rot = quad.rot;
	const Texture* texture = quad.texture;
	if (texture == nullptr) texture = m_white_texture;
	glm::vec4 uv = quad.uv;
	glm::vec4 color = quad.color;

	// Calculating the transforms
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
		* rot
		* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

	glm::vec4 quad_pos[4] = {
		{ 0.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f }
	};

	Vertex v1, v2, v3, v4;

	// Applying transforms to the vertices
	v1.pos = transform * quad_pos[0];
	v2.pos = transform * quad_pos[1];
	v3.pos = transform * quad_pos[2];
	v4.pos = transform * quad_pos[3];

	// Setting the color
	v1.color = v2.color = v3.color = v4.color = color;

	// Setting the uvs
	v1.uv = { uv.x,        uv.y        };
	v2.uv = { uv.x + uv.z, uv.y        };
	v3.uv = { uv.x + uv.z, uv.y + uv.w };
	v4.uv = { uv.x,        uv.y + uv.w };

	// Setting the texture
	v1.tex_id = v2.tex_id = v3.tex_id = v4.tex_id = texture->get_id();

	// Pushing the vertex to the buffer
	push_vertex(v1);
	push_vertex(v2);
	push_vertex(v3);
	push_vertex(v4);
}

void Renderer::push_vertex(const Vertex& v) {
	panic(
		m_buffer.size() / m_vao->get_count() < MAX_VERTEX_COUNT,
		"Max vertex count exceeded for this batch."
	);

	m_buffer.push_back(v.pos.x);
	m_buffer.push_back(v.pos.y);
	m_buffer.push_back(v.pos.z);
	m_buffer.push_back(v.color.r);
	m_buffer.push_back(v.color.g);
	m_buffer.push_back(v.color.b);
	m_buffer.push_back(v.color.a);
	m_buffer.push_back(v.uv.x);
	m_buffer.push_back(v.uv.y);
	m_buffer.push_back(v.tex_id);
}
