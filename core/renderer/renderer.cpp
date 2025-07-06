#include "renderer.h"

std::string v_src = R"(
#version 460 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;
layout(location = 3) in float tex_id;

out vec4 o_color;
out vec2 o_uv;
out float o_tex_id;

uniform mat4 mvp;

void main()
{
	o_color = color;
	o_uv = uv;
	o_tex_id = tex_id;
	gl_Position = mvp * vec4(position, 1.0f);
}
)";

std::string f_src = R"(
#version 460 core
layout(location=0) out vec4 color;

uniform sampler2D textures[32];

in vec4 o_color;
in vec2 o_uv;
in float o_tex_id;

void main() {
	int id = int(o_tex_id);
	color = texture(textures[id], o_uv) * o_color;
}
)";

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

	// NOTE(slok): Make sure to always build VAO after binding VBO
	m_vao->build();

	// Constructing Shader
	m_default_shader = new Shader(v_src, f_src, ShaderLoadType::FromStr);
	m_current_shader = m_default_shader;

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

	// Allocate CPU buffer
	m_buffer.reserve(MAX_VERTEX_COUNT * m_vao->get_count());

	// Provide texture samplers
	i32 samplers[MAX_TEXTURE_SAMPLES];
	for (i32 i = 0; i < MAX_TEXTURE_SAMPLES; i++)
		samplers[i] = i;

	// Providing samplers to the shader
	m_default_shader->bind();
	m_default_shader->set_arrayi("textures", samplers, MAX_TEXTURE_SAMPLES);

	// Enabling the Opengl Settings
	GLC(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLC(glEnable(GL_BLEND));
	GLC(glEnable(GL_DEPTH_TEST));
}

Renderer::~Renderer() {
	delete m_vbo;
	delete m_vao;
	delete m_white_texture;
	delete m_default_shader;
}

void Renderer::begin(Camera& camera) {
	camera.recalculate_view_proj();
	m_view_proj = camera.get_view_proj();
	m_buffer.clear();
}

void Renderer::end() {
	// Binding texture and shader
	m_white_texture->bind();
	m_current_shader->bind();

	// Update the view projection in shader
	m_current_shader->set_mat4f("mvp", m_view_proj, false);

	// Pushing the CPU vertex data to the GPU
	m_vbo->push_data(0, m_buffer.size() * sizeof(f32), (void*)m_buffer.data());

	// Binding the Vertex Array
	m_vao->bind();

	// TODO(slok): Add support for multiple draw types including indices one
	// Draw Call
	GLC(glDrawArrays(GL_TRIANGLES, 0, m_buffer.size() / m_vao->get_count()));
}

void Renderer::push_quad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
		* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

	glm::vec4 quad_pos[4] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f }
	};

	Vertex v1, v2, v3, v4, v5, v6;

	v1.pos = transform * quad_pos[0];
	v2.pos = transform * quad_pos[1];
	v3.pos = transform * quad_pos[2];
	v4.pos = v3.pos;
	v5.pos = transform * quad_pos[3];
	v6.pos = v1.pos;

	v1.color = v2.color = v3.color = v4.color = v5.color = v6.color = color;

	v1.uv = { 0, 0 };
	v2.uv = { 1, 0 };
	v3.uv = { 1, 1 };
	v4.uv = { 1, 1 };
	v5.uv = { 0, 1 };
	v6.uv = { 0, 0 };

	v1.tex_id = v2.tex_id = v3.tex_id = v4.tex_id = v5.tex_id = v6.tex_id = m_white_texture->get_id();

	push_vertex(v1);
	push_vertex(v2);
	push_vertex(v3);
	push_vertex(v4);
	push_vertex(v5);
	push_vertex(v6);
}

void Renderer::push_vertex(const Vertex& v) {
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

/*
Result<RenderPipeline*, std::string> rp_create(RenderPipelineSpecs* specs) {
	RenderPipeline* rp = new RenderPipeline;
	rp->max_vertices = specs->max_vertices;

	u32 vertex_stride = 0; // Size of a single vector
	rp->vertex_size = 0; // No of items in a vertex
	u32 vb_size = 0; // Total Size of The Vertex Buffer
	i32 offset = 0; // Vertex Attrib Offset Value

	// Calculating Vertex Size
	for (auto& i : specs->format) {
		vertex_stride += sizeof_gl_type(i.type) * i.count;
	}

	// Calculating max vertex buffer size
	vb_size = vertex_stride * specs->max_vertices;

	// Creating vertex array objects
	GLC(glGenVertexArrays(1, &rp->vao));
	GLC(glBindVertexArray(rp->vao));

	// Creating Vertex buffer objects
	GLC(glGenBuffers(1, &rp->vbo));
	GLC(glBindBuffer(GL_ARRAY_BUFFER, rp->vbo));
	GLC(glBufferData(GL_ARRAY_BUFFER, vb_size, nullptr, GL_DYNAMIC_DRAW));

	// Creating Adding Vertex Attrib Layout
	u32 id = 0;
	for (auto& i : specs->format) {
		GLC(glEnableVertexAttribArray(id));
		GLC(glVertexAttribPointer(id, i.count, i.type, GL_FALSE, vertex_stride, reinterpret_cast<const void*>(static_cast<uintptr_t>(offset))));
		offset += i.count * sizeof_gl_type(i.type);
		rp->vertex_size += i.count;
		id++;
	}

	// Allocating the buffer in memory
	rp->buffer = (f32*) calloc(specs->max_vertices, vertex_stride);
	rp->buffer_index = 0;

	// Will crash the whole system! TODO do error handling here
	std::string vs = specs->shaders.vertex_shader;
	std::string fs = specs->shaders.fragment_shader;
	rp->shader = new Shader(vs, fs, ShaderLoadType::FromFile);

	// Generating white texture
	u32 data = 0xffffffff;
	rp->white_texture = new Texture(
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

	// Enabling depth testing
	GLC(glEnable(GL_DEPTH_TEST));

	// Enabling alpha blending
	//GLC(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
	GLC(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLC(glEnable(GL_BLEND));

	return rp;
}

void rp_destroy(RenderPipeline* rp) {
	GLC(glDeleteVertexArrays(1, &rp->vao));
	GLC(glDeleteBuffers(1, &rp->vbo));
	delete rp->white_texture;
	delete rp->shader;
	free(rp->buffer);
}

void rp_begin(RenderPipeline* rp) {
	rp->white_texture->bind();
	GLC(glBindVertexArray(rp->vao));
	GLC(glBindBuffer(GL_ARRAY_BUFFER, rp->vbo));
	rp->shader->bind();
	// GLC(glUseProgram(rp->shader));

	// Reseting the buffer pointer
	rp->buffer_index = 0;
}

void rp_end(RenderPipeline* rp) {
	GLC(glBufferSubData(GL_ARRAY_BUFFER, 0, rp->buffer_index * sizeof(f32), rp->buffer));
	GLC(glDrawArrays(GL_TRIANGLES, 0, rp->buffer_index / rp->vertex_size));
}

void rp_push_vertices(RenderPipeline* rp, const Vertices& vertices) {
	// Manually copying data to the buffer
	for (f32 i : vertices) {

		// Handling buffer overflow
		if (rp->buffer_index / rp->vertex_size >= rp->max_vertices) {
			rp_end(rp);
			rp_begin(rp);
		}

		rp->buffer[rp->buffer_index++] = i;
	}
}

Vertices rp_create_quad(glm::vec3 pos, glm::vec2 size, glm::vec4 color) {
	glm::vec3 p1, p2, p3, p4, p5, p6;

	p1 = { pos.x, pos.y, pos.z };
	p2 = { pos.x + size.x, pos.y, pos.z };
	p3 = { pos.x + size.x, pos.y + size.y, pos.z };
	p4 = { pos.x + size.x, pos.y + size.y, pos.z };
	p5 = { pos.x, pos.y + size.y, pos.z };
	p6 = { pos.x, pos.y, pos.z };

	Vertices vertices = {
		p1.x, p1.y, p1.z, color.r, color.g, color.b, color.a, 0.0f, 0.0f, 0.0f,
		p2.x, p2.y, p2.z, color.r, color.g, color.b, color.a,0.0f, 0.0f, 0.0f,
		p3.x, p3.y, p3.z, color.r, color.g, color.b, color.a,0.0f, 0.0f, 0.0f,
		p4.x, p4.y, p4.z, color.r, color.g, color.b, color.a,0.0f, 0.0f, 0.0f,
		p5.x, p5.y, p5.z, color.r, color.g, color.b, color.a,0.0f, 0.0f, 0.0f,
		p6.x, p6.y, p6.z, color.r, color.g, color.b, color.a,0.0f, 0.0f, 0.0f,
	};

	return vertices;
}

Vertices rp_create_quad(glm::vec3 pos, glm::vec2 size, glm::vec4 color, u32 tex_id, glm::vec4 tex_coord, glm::mat4 rot) {
	// Creating positions
	glm::vec4 _p1, _p2, _p3, _p4, _p5, _p6;
	glm::vec3 p1, p2, p3, p4, p5, p6;

	// Rotating over origin
	_p1 = rot * glm::vec4(-size.x / 2, -size.y / 2, 0.0f, 1.0f);
	_p2 = rot * glm::vec4( size.x / 2, -size.y / 2, 0.0f, 1.0f);
	_p3 = rot * glm::vec4( size.x / 2,  size.y / 2, 0.0f, 1.0f);
	_p4 = rot * glm::vec4( size.x / 2,  size.y / 2, 0.0f, 1.0f);
	_p5 = rot * glm::vec4(-size.x / 2,  size.y / 2, 0.0f, 1.0f);
	_p6 = rot * glm::vec4(-size.x / 2, -size.y / 2, 0.0f, 1.0f);

	p1 = glm::vec3(_p1) / _p1.w;
	p2 = glm::vec3(_p2) / _p2.w;
	p3 = glm::vec3(_p3) / _p3.w;
	p4 = glm::vec3(_p4) / _p4.w;
	p5 = glm::vec3(_p5) / _p5.w;
	p6 = glm::vec3(_p6) / _p6.w;

	// Shifting to the desired position
	p1 += glm::vec3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
	p2 += glm::vec3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
	p3 += glm::vec3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
	p4 += glm::vec3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
	p5 += glm::vec3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
	p6 += glm::vec3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);

	// p1 = { pos.x, pos.y, pos.z };
	// p2 = { pos.x + size.x, pos.y, pos.z };
	// p3 = { pos.x + size.x, pos.y + size.y, pos.z };
	// p4 = { pos.x + size.x, pos.y + size.y, pos.z };
	// p5 = { pos.x, pos.y + size.y, pos.z };
	// p6 = { pos.x, pos.y, pos.z };

	// Creating tex coords
	glm::vec2 t1, t2, t3, t4, t5, t6;

	t1 = { tex_coord.x, tex_coord.y };
	t2 = { tex_coord.x + tex_coord.z, tex_coord.y };
	t3 = { tex_coord.x + tex_coord.z, tex_coord.y + tex_coord.w };
	t4 = { tex_coord.x + tex_coord.z, tex_coord.y + tex_coord.w };
	t5 = { tex_coord.x, tex_coord.y + tex_coord.w };
	t6 = { tex_coord.x, tex_coord.y };

	f32 id = tex_id;
	Vertices vertices = {
		p1.x, p1.y, p1.z, color.r, color.g, color.b, color.a, t1.x, t1.y, id,
		p2.x, p2.y, p2.z, color.r, color.g, color.b, color.a, t2.x, t2.y, id,
		p3.x, p3.y, p3.z, color.r, color.g, color.b, color.a, t3.x, t3.y, id,
		p4.x, p4.y, p4.z, color.r, color.g, color.b, color.a, t4.x, t4.y, id,
		p5.x, p5.y, p5.z, color.r, color.g, color.b, color.a, t5.x, t5.y, id,
		p6.x, p6.y, p6.z, color.r, color.g, color.b, color.a, t6.x, t6.y, id,
	};

	return vertices;
}

Vertices rp_create_text(Font* font, const std::string& text, glm::vec3 pos, glm::vec4 color) {
	Vertices buffer;

	glm::vec3 quad_p = pos;
	for (char c : text) {
		if (c == '\n') {
			glm::vec2 size = font_calc_size(font, " ");

			// Reset the x and increase the y
			quad_p.x = pos.x;
			quad_p.y += size.y;
			continue;
		}

		panic(font->glyphs.find(c) != font->glyphs.end(), "Cannot find the character in glyph table: %c", c);

		auto [uv, size] = font->glyphs[c];

		// Generating quad for each character
		Vertices v = rp_create_quad(quad_p, size, color, font->atlas->get_id(), uv);
		quad_p.x += size.x;

		// Copying the vertices to buffer
		buffer.insert(buffer.end(), v.begin(), v.end());
	}

	return buffer;
}

*/
