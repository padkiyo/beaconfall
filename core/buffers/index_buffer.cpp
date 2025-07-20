#include "index_buffer.h"

IndexBuffer::IndexBuffer(i32 index_count)
	: m_index_count(index_count) {
	GLC(glGenBuffers(1, &m_id));
	GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

IndexBuffer::~IndexBuffer() {
	GLC(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::bind() {
	GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void IndexBuffer::unbind() {
	GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::generate_quad_indices() {
	u32 indices[m_index_count];

	for (i32 offset = 0, i = 0; i < m_index_count; i += 6) {
		indices[0 + i] = 0 + offset;
		indices[1 + i] = 1 + offset;
		indices[2 + i] = 2 + offset;

		indices[3 + i] = 2 + offset;
		indices[4 + i] = 3 + offset;
		indices[5 + i] = 0 + offset;
		offset += 4;
	}

	bind();
	GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index_count * sizeof(u32), indices, GL_STATIC_DRAW));
}
