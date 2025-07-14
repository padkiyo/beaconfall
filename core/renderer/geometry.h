#pragma once

#include "common.h"
#include "texture/texture.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec2 uv;
	f32 tex_id;
};

struct Quad {
	glm::vec3 pos;
	glm::vec2 size;
	glm::mat4 rot;
	const Texture* texture;
	glm::vec4 uv;
	glm::vec4 color;
};

struct Light {
	glm::vec2 pos;
	f32 radius;
	f32 intensity;
	f32 dir;
	f32 fov;
	glm::vec4 color;
};

