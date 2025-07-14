#pragma once

#include "common.h"

struct TextureFilter {
	u32 min_filter;
	u32 mag_filter;
	u32 wrap_s;
	u32 wrap_t;
	u32 flip;
};

struct TextureFormat {
	u32 internal_format;
	u32 format;
};

class Texture {
public:
	Texture(const std::string& filepath, const TextureFilter& filter);
	Texture(u32 width, u32 height, void* data, const TextureFormat& format, const TextureFilter& filter);
	~Texture();

	void bind();
	void unbind();

	// Note(slok): It is assumed that the format of the data is same as the format of the texture
	void update(i32 lod, i32 x_offset, i32 y_offset, u32 width, u32 height, void* data);
	void clear();

	//---------
	// Getters
	//---------

	inline u32 get_id() const { return m_id; }
	inline u32 get_width() const { return m_width; }
	inline u32 get_height() const { return m_height; }

private:
	u32 m_id, m_width, m_height;
	TextureFormat m_format;
};
