#pragma once

#include "common.h"

typedef u32 Shader;

/*
 * This doesnt provide functions to bind and unbind and also uniforms.
 * Bindings and Uniforms will be handled using raw opengl calls
 */

// Creates shader from file
Result<Shader, std::string> shader_create(const std::string& v_src_path, const std::string& f_src_path);

// Creates shader from strings
Result<Shader, std::string> shader_create_raw(const std::string& v_src, const std::string& f_src);

// Use this to delete the shader
void shader_destroy(Shader id);

// Compiles individual shader source
Result<u32, std::string> shader_compile(u32 type, const char* shader_src);
