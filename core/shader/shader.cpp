#include "shader.h"

Shader::Shader(const std::string& v_src, const std::string& f_src, ShaderLoadType ltype) {
	switch (ltype) {
		case ShaderLoadType::FromFile:
			load_from_file(v_src, f_src);
			break;
		case ShaderLoadType::FromStr:
			load_from_str(v_src, f_src);
			break;
		default:
			panic(false, "Unhandled ShaderLoadType");
	}
}

Shader::~Shader() {
	GLC(glDeleteProgram(m_id));
}

void Shader::bind() {
	GLC(glUseProgram(m_id));
}

void Shader::unbind() {
	GLC(glUseProgram(0));
}

i32 Shader::get_uniform_loc(const std::string& name) {
	if (m_uniforms.find(name) != m_uniforms.end())
		return m_uniforms[name];

	i32 loc = GLC(glGetUniformLocation(m_id, name.c_str()));
	if (loc == -1)
		log_error("Cannot find uniform: %s\n", name.c_str());

	// Caching the uniform locations
	m_uniforms.insert_or_assign(name, loc);

	return loc;
}

void Shader::set_1i(const std::string& name, i32 x) {
	i32 loc = get_uniform_loc(name);
	GLC(glUniform1i(loc, x));
}

void Shader::set_1f(const std::string& name, f32 x) {
	i32 loc = get_uniform_loc(name);
	GLC(glUniform1f(loc, x));
}

void Shader::set_2f(const std::string& name, f32 x, f32 y) {
	i32 loc = get_uniform_loc(name);
	GLC(glUniform2f(loc, x, y));
}

void Shader::set_3f(const std::string& name, f32 x, f32 y, f32 z) {
	i32 loc = get_uniform_loc(name);
	GLC(glUniform3f(loc, x, y, z));
}

void Shader::set_4f(const std::string& name, f32 x, f32 y, f32 z, f32 w) {
	i32 loc = get_uniform_loc(name);
	GLC(glUniform4f(loc, x, y, z, w));
}

void Shader::set_arrayi(const std::string& name, i32* value, u32 count) {
	i32 loc = get_uniform_loc(name);
	GLC(glUniform1iv(loc, count, value));
}

void Shader::set_mat4f(const std::string& name, const glm::mat4& value, b32 transpose) {
	i32 loc = get_uniform_loc(name);
	GLC(glUniformMatrix4fv(loc, 1, transpose, &value[0][0]));
}

void Shader::load_from_file(const std::string& v_path, const std::string& f_path) {
	// Opening vertex shader file
	std::ifstream v_src_file(v_path);
	panic(v_src_file.is_open(), "Failed to open Shader File: %s", v_path.c_str());

	// Opening fragment shader file
	std::ifstream f_src_file(f_path);
	panic(f_src_file.is_open(), "Failed to open Shader File: %s", f_path.c_str());

	// Reading vertex shader src
	std::ostringstream ss;
	ss << v_src_file.rdbuf();
	std::string v_src = ss.str();

	// Cleaning the buffer
	ss.str("");
	ss.clear();

	// Reading fragment shader src
	ss << f_src_file.rdbuf();
	std::string f_src = ss.str();

	// Closing the files
	v_src_file.close();
	f_src_file.close();

	// Loading shader from strings
	load_from_str(v_src, f_src);
}

void Shader::load_from_str(const std::string& v_src, const std::string& f_src) {
	u32 program = GLC(glCreateProgram());

	u32 vs = compile(GL_VERTEX_SHADER, v_src.c_str());
	u32 fs = compile(GL_FRAGMENT_SHADER, f_src.c_str());

	// Attaching shader
	GLC(glAttachShader(program, vs));
	GLC(glAttachShader(program, fs));
	GLC(glLinkProgram(program));
	GLC(glValidateProgram(program));

	GLC(glDeleteShader(vs));
	GLC(glDeleteShader(fs));

	// Setting up the shader id
	m_id = program;
}

u32 Shader::compile(u32 type, const char* src) {
	u32 id = GLC(glCreateShader(type));

	GLC(glShaderSource(id, 1, &src, NULL));
	GLC(glCompileShader(id));

	// Checking error in shader
	i32 result;
	GLC(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		i32 length;
		GLC(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char message[length];
		GLC(glGetShaderInfoLog(id, length, &length, message));

		std::stringstream ss;
		ss
			<< "Failed to compile ["
			<< (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
			<< " shader]\n"
			<< message
			<< "\n";
		std::string error = ss.str();
		panic(false, "%s", error.c_str());
	}

	return id;
}
