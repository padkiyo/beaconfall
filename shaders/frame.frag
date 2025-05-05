#version 460 core
layout(location = 0) out vec4 color;

in vec2 o_uv;
uniform sampler2D textures;
uniform float bnw;

void main() {
	//color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 tex = texture(textures, o_uv);
	if (bnw == 1.0f)
	{
		float avg = ( tex.x + tex.y + tex.z ) / 3;
		color = vec4(avg, avg, avg, 1.0f);
	}
	else{
		color = tex;
	}
}
