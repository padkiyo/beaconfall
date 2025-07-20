#version 460 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;
layout(location = 3) in float tex_id;
layout(location = 4) in vec4 overlay;

out vec4 o_color;
out vec2 o_uv;
out float o_tex_id;
out vec4 o_overlay;

uniform mat4 mvp;

void main() {
	o_color = color;
	o_uv = uv;
	o_tex_id = tex_id;
	o_overlay = overlay;
	gl_Position = mvp * vec4(position, 1.0f);
}
