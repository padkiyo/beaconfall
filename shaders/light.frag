#version 460 core
layout(location=0) out vec4 color;

struct Light {
	vec2 pos;
	float radius;
	float intensity;
	float dir;
	float fov;
	float fall_off;
	vec4 color;
};

uniform Light lights[32];
uniform int light_count;
uniform sampler2D textures[32];
uniform vec2 dim;
uniform vec2 pixel_size;
uniform vec3 ambient_color;

in vec4 o_color;
in vec2 o_uv;
in float o_tex_id;
in mat4 o_mvp;

vec2 rotate(vec2 v, float angle) {
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	return vec2(
		v.x * cosAngle - v.y * sinAngle,
		v.x * sinAngle + v.y * cosAngle
	);
}

void main() {
	vec4 final_color = vec4(ambient_color, 1);
	vec2 aspect = vec2(dim.x / dim.y, 1.0);

	for (int i = 0; i < light_count; i++) {
		// Calculating pixelated uv
		vec2 block_coord = floor(gl_FragCoord.xy / pixel_size) * pixel_size;
		vec2 uv = ((block_coord - 0.5 * dim) / dim * 2.0) * aspect;

		// Calculate the light position with camera perspective
		vec4 nl = vec4(lights[i].pos, 0, 1);
		vec4 tl = o_mvp * nl;
		vec2 pos = (tl.xy / tl.w) * aspect;

		// Rotating the uv with light direction
		vec2 frag_pos = uv - pos;
		vec2 rot_frag_pos = rotate(frag_pos, lights[i].dir);
		uv = rot_frag_pos + pos;

		// Calculating fall ofs
		float dist = length(frag_pos);
		float radial_fall_off = pow(
			clamp(1.0 - dist / lights[i].radius, 0.0, 1.0),
			lights[i].fall_off
		);
		float angle = abs(atan(uv.y - pos.y, uv.x - pos.x));
		float angular_fall_off = smoothstep(lights[i].fov, 0, angle);

		final_color += lights[i].color
		* lights[i].intensity
		* radial_fall_off
		* angular_fall_off;
	}

	int id = int(o_tex_id);
	color = texture(textures[id], o_uv) * final_color;
}
