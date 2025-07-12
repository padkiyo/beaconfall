#include "ui.h"

UI::UI(Renderer* renderer, const glm::vec2& resolution)
	: m_renderer(renderer), m_res(resolution) {
	m_camera = new Camera(glm::vec3(0,0,0), {
		.left = 0,
		.right = m_res.x,
		.top = m_res.y,
		.bottom = 0,
		.near = 0,
		.far = 1000,
	});

	m_shader = new Shader("./shaders/color.vert", "./shaders/color.frag", ShaderLoadType::FromFile);

	// Provide texture samplers
	i32 samplers[MAX_TEXTURE_SAMPLES];
	for (i32 i = 0; i < MAX_TEXTURE_SAMPLES; i++)
		samplers[i] = i;

	// Providing samplers to the shaders
	m_shader->bind();
	m_shader->set_arrayi("textures", samplers, MAX_TEXTURE_SAMPLES);
}

UI::~UI() {
	delete m_camera;
	delete m_shader;
}

void UI::begin() {
	m_renderer->begin_pass({0,0}, m_res);

	m_camera->recalculate_view_proj();
	glm::mat4 view_proj = m_camera->get_view_proj();

	m_shader->bind();
	m_shader->set_mat4f("mvp", view_proj, false);
}

void UI::end() {
	m_renderer->end_pass();
}

void UI::text(const std::string& msg, const glm::vec2& pos, const Style& style) {
	panic(style.font, "Font is not provided in style for text");

	glm::vec2 quad_p = pos;

	for (char c : msg) {

		// Adjusting for new line character
		if (c == '\n') {
			glm::vec2 size = style.font->pixel_size();

			// Reset the x and increase the y
			quad_p.x = pos.x;
			quad_p.y += size.y;
			continue;
		}

		// Getting the glyph for the character
		Glyph glyph = style.font->get_glyph(c);

		// Rendering the corresponding glyph
		m_renderer->push_quad(Quad {
			{ quad_p.x, quad_p.y, 0 },
			glyph.size,
			glm::mat4(1),
			&style.font->get_atlas(),
			glyph.uv,
			style.fg_color
		});

		quad_p.x += glyph.size.x;
	}
}
