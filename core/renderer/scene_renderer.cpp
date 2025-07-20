#include "scene_renderer.h"

SceneRenderer::SceneRenderer(Renderer* renderer, const glm::vec2& res)
	: m_renderer(renderer) {
	// Constructing Shader
	m_color_shader = new Shader("./shaders/color.vert", "./shaders/color.frag", ShaderLoadType::FromFile);
	m_light_shader = new Shader("./shaders/light.vert", "./shaders/light.frag", ShaderLoadType::FromFile);

	// Provide texture samplers
	i32 samplers[MAX_TEXTURE_SAMPLES];
	for (i32 i = 0; i < MAX_TEXTURE_SAMPLES; i++)
		samplers[i] = i;

	// Providing samplers to the shaders
	m_color_shader->bind();
	m_color_shader->set_arrayi("textures", samplers, MAX_TEXTURE_SAMPLES);

	m_light_shader->bind();
	m_light_shader->set_arrayi("textures", samplers, MAX_TEXTURE_SAMPLES);

	// Creating a fake camera
	m_camera = new Camera(glm::vec3(0,0,0), {
		.left = 0,
		.right = res.x,
		.top = res.y,
		.bottom = 0,
		.near = 0,
		.far = 1000,
	});
}

SceneRenderer::~SceneRenderer() {
	delete m_color_shader;
	delete m_light_shader;
	delete m_camera;
}

void SceneRenderer::render_scene(const Scene& scene, Camera& camera, const glm::vec2& res) {
	camera.recalculate_view_proj();
	glm::mat4 view_proj = camera.get_view_proj();

	m_camera->recalculate_view_proj();
	glm::mat4 fake_view_proj = m_camera->get_view_proj();

	/*
	 * Color Render Pass
	 */

	m_renderer->begin_pass({0,0}, res);
	{
		m_renderer->clear({0.5,0.5,0.5,1});

		m_color_shader->bind();
		m_color_shader->set_mat4f("mvp", view_proj, false);

		// Pushing quads from scene to renderer
		for (auto quad : scene.get_quads()) {
			m_renderer->push_quad(quad);
		}
	}
	m_renderer->end_pass();


	/*
	 * Light Render Pass
	 */

	m_renderer->begin_pass({0,0}, res);
	{
		m_renderer->clear({0.5,0.5,0.5,1});

		m_light_shader->bind();
		m_light_shader->set_mat4f("mvp", fake_view_proj, false);
		m_light_shader->set_mat4f("light_mvp", view_proj, false);

		// Getting lights from scene
		const std::vector<Light>& lights = scene.get_lights();
		const glm::vec2& light_pixel_size = scene.get_light_pixel_size();
		const glm::vec3& ambient_color = scene.get_ambient_color();

		// Provide lights to the shader
		for (i32 i = 0; i < lights.size(); i++) {
			m_light_shader->set_2f(std::format("lights[{}].pos", i), lights[i].pos.x, lights[i].pos.y);
			m_light_shader->set_1f(std::format("lights[{}].radius", i), lights[i].radius);
			m_light_shader->set_1f(std::format("lights[{}].intensity", i), lights[i].intensity);
			m_light_shader->set_1f(std::format("lights[{}].dir", i), lights[i].dir);
			m_light_shader->set_1f(std::format("lights[{}].fov", i), lights[i].fov);
			m_light_shader->set_1f(std::format("lights[{}].fall_off", i), lights[i].fall_off);
			m_light_shader->set_4f(std::format("lights[{}].color", i), lights[i].color.r, lights[i].color.g, lights[i].color.b, lights[i].color.a);
		}

		m_light_shader->set_1i("light_count", lights.size());
		m_light_shader->set_2f("dim", res.x, res.y);
		m_light_shader->set_2f("pixel_size", light_pixel_size.x, light_pixel_size.y);
		m_light_shader->set_3f("ambient_color", ambient_color.r, ambient_color.g, ambient_color.b);
	}
	m_renderer->end_pass();
}
