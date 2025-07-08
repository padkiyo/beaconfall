#include "renderer.h"

Renderer::Renderer(const glm::vec2& resolution)
	: m_res(resolution) {
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

	// NOTE(slok): Make sure to always build VAO only after binding VBO
	m_vao->build();

	// Allocate CPU buffer
	m_buffer.reserve(MAX_VERTEX_COUNT * m_vao->get_count());
	m_lights.reserve(MAX_LIGHTS);

	// Building the framebuffers
	m_color_pass = new FrameBuffer(m_res.x, m_res.y);
	m_light_pass = new FrameBuffer(m_res.x, m_res.y);

	// Constructing Shader
	m_color_shader = new Shader("./shaders/color.vert", "./shaders/color.frag", ShaderLoadType::FromFile);
	m_light_shader = new Shader("./shaders/light.vert", "./shaders/light.frag", ShaderLoadType::FromFile);
	m_final_shader = new Shader("./shaders/plain.vert", "./shaders/color.frag", ShaderLoadType::FromFile);

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
	m_color_shader->bind();
	m_color_shader->set_arrayi("textures", samplers, MAX_TEXTURE_SAMPLES);

	m_light_shader->bind();
	m_light_shader->set_arrayi("textures", samplers, MAX_TEXTURE_SAMPLES);

	m_final_shader->bind();
	m_final_shader->set_arrayi("textures", samplers, MAX_TEXTURE_SAMPLES);

	// Enabling the Opengl Settings
	GLC(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLC(glEnable(GL_BLEND));
	GLC(glEnable(GL_DEPTH_TEST));
}

Renderer::~Renderer() {
	delete m_vbo;
	delete m_vao;
	delete m_white_texture;

	delete m_color_shader;
	delete m_light_shader;
	delete m_final_shader;

	delete m_color_pass;
	delete m_light_pass;
}

void Renderer::begin(Camera& camera) {
	camera.recalculate_view_proj();
	m_view_proj = camera.get_view_proj();

	m_buffer.clear();
	m_lights.clear();
}

void Renderer::end() {
	// Binding common resources
	m_white_texture->bind();

	// Execute all the passes
	execute_color_pass();
	execute_light_pass();

	// Render the final texture to the screen
	GLC(glViewport(0, 0, m_res.x, m_res.y));
	GLC(glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a));
	GLC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	m_buffer.clear();

	// Binding the channels
	m_color_pass->bind_color_channel();
	m_light_pass->bind_color_channel();

	m_final_shader->bind();

	push_quad(
		{-1,-1,0},
		{2, 2},
		glm::mat4(1.0f),
		m_light_pass->get_color_texture(),
		{0,0,1,1},
		{1,1,1,1}
	);

	execute_draw_call();
}

void Renderer::execute_draw_call() {
	m_vbo->push_data(0, m_buffer.size() * sizeof(f32), (void*)m_buffer.data());
	m_vao->bind();
	GLC(glDrawArrays(GL_TRIANGLES, 0, m_buffer.size() / m_vao->get_count()));
}

void Renderer::execute_color_pass() {
	m_color_pass->bind();
	{
		GLC(glViewport(0, 0, m_res.x, m_res.y));
		GLC(glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a));
		GLC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_color_shader->bind();
		m_color_shader->set_mat4f("mvp", m_view_proj, false);

		execute_draw_call();

		m_color_shader->unbind();
	}
	m_color_pass->unbind();
}

void Renderer::execute_light_pass() {
	m_light_pass->bind();
	{
		GLC(glViewport(0, 0, m_res.x, m_res.y));
		GLC(glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a));
		GLC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_light_shader->bind();
		m_light_shader->set_mat4f("mvp", m_view_proj, false);

		// Provide lights to the shader
		for (i32 i = 0; i < m_lights.size(); i++) {
			m_light_shader->set_2f(std::format("lights[{}].pos", i), m_lights[i].pos.x, m_lights[i].pos.y);
			m_light_shader->set_1f(std::format("lights[{}].radius", i), m_lights[i].radius);
			m_light_shader->set_1f(std::format("lights[{}].intensity", i), m_lights[i].intensity);
			m_light_shader->set_1f(std::format("lights[{}].dir", i), m_lights[i].dir);
			m_light_shader->set_1f(std::format("lights[{}].fov", i), m_lights[i].fov);
			m_light_shader->set_4f(std::format("lights[{}].color", i), m_lights[i].color.r, m_lights[i].color.g, m_lights[i].color.b, m_lights[i].color.a);
		}

		m_light_shader->set_1i("light_count", m_lights.size());
		m_light_shader->set_2f("dim", m_res.x, m_res.y);
		m_light_shader->set_2f("pixel_size", m_light_pixel_size.x, m_light_pixel_size.y);
		m_light_shader->set_3f("ambient_color", m_ambient_color.r, m_ambient_color.g, m_ambient_color.b);

		// Clear the buffer
		m_buffer.clear();

		m_color_pass->bind_color_channel();
		push_quad(
			{0,0,0},
			m_res,
			glm::mat4(1.0f),
			m_color_pass->get_color_texture(),
			{0,0,1,1},
			{1,1,1,1}
		);

		execute_draw_call();

		m_light_shader->unbind();
	}
	m_light_pass->unbind();
}

void Renderer::set_clear_color(const glm::vec4& color) {
	m_clear_color = color;
}

void Renderer::push_quad(
	const glm::vec3& pos,
	const glm::vec2& size,
	const glm::mat4& rot,
	const Texture& texture,
	const glm::vec4& uv,
	const glm::vec4& color
) {
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

	Vertex v1, v2, v3, v4, v5, v6;

	// Applying transforms to the vertices
	v1.pos = transform * quad_pos[0];
	v2.pos = transform * quad_pos[1];
	v3.pos = transform * quad_pos[2];
	v4.pos = v3.pos;
	v5.pos = transform * quad_pos[3];
	v6.pos = v1.pos;

	// Setting the color
	v1.color = v2.color = v3.color = v4.color = v5.color = v6.color = color;

	// Setting the uvs
	v1.uv = { uv.x, uv.y };
	v2.uv = { uv.x + uv.z, uv.y };
	v3.uv = { uv.x + uv.z, uv.y + uv.w };
	v4.uv = { uv.x + uv.z, uv.y + uv.w };
	v5.uv = { uv.x, uv.y + uv.w };
	v6.uv = { uv.x, uv.y };

	// Setting the texture
	v1.tex_id = v2.tex_id = v3.tex_id = v4.tex_id = v5.tex_id = v6.tex_id = texture.get_id();

	// Pushing the vertex to the buffer
	push_vertex(v1);
	push_vertex(v2);
	push_vertex(v3);
	push_vertex(v4);
	push_vertex(v5);
	push_vertex(v6);
}

void Renderer::push_vertex(const Vertex& v) {
	// TODO(slok): Add Support for dyanmic batch rendering
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

void Renderer::push_light(const Light& l) {
	// TODO(slok): Do a dynamic rendering approach
	panic(
		m_lights.size() < MAX_LIGHTS,
		"Max light exceeded for this batch."
	);

	m_lights.push_back(l);
}

void Renderer::set_ambient_color(const glm::vec3& color) {
	m_ambient_color = color;
}

void Renderer::set_light_pixel_size(const glm::vec2& size) {
	m_light_pixel_size = size;
}
