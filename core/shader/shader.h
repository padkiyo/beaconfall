#pragma once

#include "common.h"

typedef u32 Shader;

Result<Shader, std::string> shader_create(const std::string& v_src_path, const std::string& f_src_path);
Result<Shader, std::string> shader_create_raw(const std::string& v_src, const std::string& f_src);
void shader_destroy(Shader id);
Result<u32, std::string> shader_compile(u32 type, const char* shader_src);
