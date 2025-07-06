#include "vertex_array.h"

VertexArray::VertexArray() {
	GLC(glGenVertexArrays(1, &m_id));
	GLC(glBindVertexArray(m_id));
}

VertexArray::~VertexArray() {
	GLC(glDeleteVertexArrays(1, &m_id));
}

void VertexArray::build() {
	i32 id = 0;
	for (auto& layout : m_layouts) {
		// NOTE(slok): Here the normalized variable is hardcoded to false.
		GLC(glEnableVertexAttribArray(id));
		GLC(glVertexAttribPointer(
			id, layout.count, layout.type, GL_FALSE, m_stride,
			reinterpret_cast<const void*>(static_cast<uintptr_t>(layout.offset)) // What the hell is this casting shit
		));
		id++;
	}
}

void VertexArray::bind() {
	GLC(glBindVertexArray(m_id));
}

void VertexArray::unbind() {
	GLC(glBindVertexArray(0));
}
