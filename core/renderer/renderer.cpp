#include "renderer.h"

Result<RenderPipeline, const char*>  rp_create(RenderPipelineSpecs* specs) {
	RenderPipeline rp;

	u32 vector_stride = 0; // Size of a single vector
	u32 vb_size = 0; // Total Size of The Vertex Buffer
	u32 offset = 0; // Vertex Attrib Offset Value

	// Calculating Vertex Size
	for (auto i = begin(specs->format); i != end(specs->format); ++i) {
		vector_stride += sizeof(i->type) * i->count;
	}

	// TODO rename vector_stride to vertex_stride :D
	vb_size = vector_stride * specs->max_vertices;

	// Creating vertex array objects
	glc(glGenVertexArrays(1, &rp.vao));
	glc(glBindVertexArray(rp.vao));

	// Creating Vertex buffer objects
	glc(glGenBuffers(1, &rp.vbo));
	glc(glBindBuffer(GL_ARRAY_BUFFER, rp.vbo));
	glc(glBufferData(GL_ARRAY_BUFFER, vb_size, nullptr, GL_DYNAMIC_DRAW));

	// Creating Adding Vertex Attrib Layout
	u32 id = 0;
	for (auto i = begin(specs->format); i != end(specs->format); ++i) {

		glc(glEnableVertexAttribArray(id));
		glc(glVertexAttribPointer(id, i->count, i->type, GL_FALSE, vector_stride, (const void*) offset));

		offset += i->count * sizeof(i->type);
		id += 1;
	}

	rp.buffer = (f32*) calloc(specs->max_vertices, vector_stride);
	rp.buffer_index = 0;
	rp.buffer_actual_size = 0;

	std::string vs = specs->shaders.vertex_shader;
	std::string fs = specs->shaders.fragment_shader;

	// Will crash the whole system! TODO do error handling here
	rp.shader = shader_create(vs, fs).unwrap();
	return rp;
}

void rp_begin(RenderPipeline* rp){
	glc(glBindVertexArray(rp->vao));
	glc(glBindBuffer(GL_ARRAY_BUFFER, rp->vbo));
	glc(glUseProgram(rp->shader));
}

void rp_push_vertex(RenderPipeline* rp, std::vector<f32> vertices) {
	u32 index = rp->buffer_index;
	// TODO add a buffer overflow mechanism

	// Manually copying data to the buffer
	for (auto i = begin(vertices); i != end(vertices); ++i) {
		rp->buffer[index] = *i;
		rp->buffer_actual_size += sizeof(*i);
		++index;
	}
	rp->buffer_index = index;
}

std::vector<float> create_quad(float x, float y, float size, glm::vec4 color){
		std::vector<float> vertices = {};

		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertices.push_back(color.x);
		vertices.push_back(color.y);
		vertices.push_back(color.z);
		vertices.push_back(color.w);


		vertices.push_back(x + size);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertices.push_back(color.x);
		vertices.push_back(color.y);
		vertices.push_back(color.z);
		vertices.push_back(color.w);

		vertices.push_back(x + size);
		vertices.push_back(y + size);
		vertices.push_back(0.0f);
		vertices.push_back(color.x);
		vertices.push_back(color.y);
		vertices.push_back(color.z);
		vertices.push_back(color.w);


		vertices.push_back(x);
		vertices.push_back(y + size);
		vertices.push_back(0.0f);
		vertices.push_back(color.x);
		vertices.push_back(color.y);
		vertices.push_back(color.z);
		vertices.push_back(color.w);

		return vertices;
}

std::vector<float> create_quad_texture(float x, float y, float size, std::vector<glm::vec2> texture_coords){
		std::vector<float> vertices = {};

		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertices.push_back(texture_coords[0].x);
		vertices.push_back(texture_coords[0].y);


		vertices.push_back(x + size);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertices.push_back(texture_coords[1].x);
		vertices.push_back(texture_coords[1].y);

		vertices.push_back(x + size);
		vertices.push_back(y + size);
		vertices.push_back(0.0f);
		vertices.push_back(texture_coords[2].x);
		vertices.push_back(texture_coords[2].y);


		vertices.push_back(x);
		vertices.push_back(y + size);
		vertices.push_back(0.0f);
		vertices.push_back(texture_coords[3].x);
		vertices.push_back(texture_coords[3].y);

		return vertices;
}

void rp_push_quad(RenderPipeline* rp, glm::vec2 pos, f32 size, glm::vec4 color){
	rp_push_vertex(rp, create_quad(pos.x, pos.y, size, color));
}

void rp_push_quad_tex(RenderPipeline* rp, glm::vec2 pos, f32 size, std::vector<glm::vec2> texture_coords){
	rp_push_vertex(rp, create_quad_texture(pos.x, pos.y, size, texture_coords));
}

void rp_end(RenderPipeline* rp)
{
	glc(glBufferSubData(GL_ARRAY_BUFFER, 0, rp->buffer_actual_size, rp->buffer));
	glc(glDrawArrays(GL_TRIANGLES, 0, rp->buffer_index + 1));
}

