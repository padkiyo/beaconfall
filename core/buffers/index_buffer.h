#pragma once

#include "common.h"

class IndexBuffer {
public:
	IndexBuffer(i32 index_count);
	~IndexBuffer();

	void bind();
	void unbind();
	void generate_quad_indices();

	u32 get_id() const { return m_id; }
	i32 get_index_count() const { return m_index_count; }

private:
	u32 m_id;
	i32 m_index_count;
};
