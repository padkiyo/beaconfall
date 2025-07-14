#pragma once

#include "common.h"

struct VertexLayout {
	i32 count;
	u32 type;
	i32 offset;
};

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	template<typename T>
	void push_layout(int count) {
		m_layouts.push_back({
			.count = count,
			.type = get_gl_type<T>(),
			.offset = m_stride,
		});
		m_stride += count * sizeof(T);
		m_count += count;
	}

	void build();
	void bind();
	void unbind();

	inline i32 get_stride() const { return m_stride; }
	inline i32 get_count()  const { return m_count; }

private:
	template <typename T>
	u32 get_gl_type() {
		if (typeid(T) == typeid(f32)) return GL_FLOAT;
		if (typeid(T) == typeid(i32)) return GL_INT;
		if (typeid(T) == typeid(u32)) return GL_UNSIGNED_INT;
		panic(false, "Unhandled type");
	}

private:
	u32 m_id;
	i32 m_stride = 0;
	i32 m_count = 0;
	std::vector<VertexLayout> m_layouts;
};
