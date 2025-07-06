#pragma once

#include "common.h"

class VertexBuffer {
public:
	VertexBuffer(ssize_t size, void* data, u32 draw_type);
	~VertexBuffer();

	void bind();
	void unbind();
	void push_data(i32 offset, i32 size, void* data);

	inline u32 get_id() const { return m_id; }

private:
	u32 m_id, m_draw_type;
	ssize_t m_size;
};
