#include "vertex_buffer.h"

VertexBuffer::VertexBuffer(ssize_t size, void* data, u32 draw_type)
	: m_size(size), m_draw_type(draw_type) {
	GLC(glGenBuffers(1, &m_id));
	GLC(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	GLC(glBufferData(GL_ARRAY_BUFFER, m_size, data, m_draw_type));
}

VertexBuffer::~VertexBuffer() {
	GLC(glDeleteBuffers(1, &m_id));
}

void VertexBuffer::bind() {
	GLC(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void VertexBuffer::unbind() {
	GLC(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::push_data(i32 offset, i32 size, void* data) {
	panic(m_draw_type == GL_DYNAMIC_DRAW, "Cannot push data in static buffer.");
	GLC(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	GLC(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}
