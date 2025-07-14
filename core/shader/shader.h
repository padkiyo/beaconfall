#pragma once

#include "common.h"

enum class ShaderLoadType {
	FromFile,
	FromStr,
};

class Shader {
public:
	Shader(const std::string& v_src, const std::string& f_src, ShaderLoadType ltype);
	~Shader();

	void bind();
	void unbind();

	//---------
	// Getters
	//---------

	inline u32 get_id() const { return m_id; }
	i32 get_uniform_loc(const std::string& name);

	//---------
	// Setters
	//---------

	void set_1i(const std::string& name, i32 x);
	void set_1f(const std::string& name, f32 x);
	void set_2f(const std::string& name, f32 x, f32 y);
	void set_3f(const std::string& name, f32 x, f32 y, f32 z);
	void set_4f(const std::string& name, f32 x, f32 y, f32 z, f32 w);
	void set_arrayi(const std::string& name, i32* value, u32 count);
	void set_mat4f(const std::string& name, const glm::mat4& value, b32 transpose);

private:
	void load_from_file(const std::string& v_path, const std::string& f_path);
	void load_from_str(const std::string& v_src, const std::string& f_src);
	u32 compile(u32 type, const char* src);

private:
	u32 m_id;
	std::unordered_map<std::string, i32> m_uniforms;
};
