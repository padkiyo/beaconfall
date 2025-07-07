#version 460 core
layout(location=0) out vec4 color;

uniform sampler2D textures[32];

in vec4 o_color;
in vec2 o_uv;
in float o_tex_id;

void main() {
	int id = int(o_tex_id);
	color = texture(textures[id], o_uv) * o_color;
}
