#include "shader.h"

Result<Shader, std::string> shader_create(const std::string& v_src_path, const std::string& f_src_path) {
	// Opening vertex shader file
	std::ifstream v_src_file(v_src_path);
	if (!v_src_file.is_open()) {
		std::stringstream ss;
		ss << "Failed to open shader file: " << v_src_path << std::endl;
		return ss.str();
	}

	// Opening fragment shader file
	std::ifstream f_src_file(f_src_path);
	if (!f_src_file.is_open()) {
		std::stringstream ss;
		ss << "Failed to open shader file: " << f_src_path << std::endl;
		return ss.str();
	}

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

	// Creating shader from strings
	return shader_create_raw(v_src, f_src);
}

Result<Shader, std::string> shader_create_raw(const std::string& v_src, const std::string& f_src) {
	u32 program = glc(glCreateProgram());

	u32 vs = xx(shader_compile(GL_VERTEX_SHADER,	 v_src.c_str()));
	u32 fs = xx(shader_compile(GL_FRAGMENT_SHADER, f_src.c_str()));

	// Attaching shader
	glc(glAttachShader(program, vs));
	glc(glAttachShader(program, fs));
	glc(glLinkProgram(program));
	glc(glValidateProgram(program));

	glc(glDeleteShader(vs));
	glc(glDeleteShader(fs));

	return program;
}

void shader_destroy(Shader id) {
	glc(glDeleteProgram(id));
}

Result<u32, std::string> shader_compile(u32 type, const char* shader_src) {
	u32 id = glc(glCreateShader(type));

	glc(glShaderSource(id, 1, &shader_src, NULL));
	glc(glCompileShader(id));

	// Checking error in shader
	i32 result;
	glc(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		i32 length;
		glc(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char message[length];
		glc(glGetShaderInfoLog(id, length, &length, message));

		std::stringstream ss;
		ss
			<< "Failed to compile ["
			<< (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
			<< " shader]\n"
			<< message
			<< "\n";

		return ss.str();
	}
	return id;
}
