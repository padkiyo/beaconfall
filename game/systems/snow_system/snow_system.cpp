#include "snow_system.h"

SnowSystem::SnowSystem(const glm::vec2& res): m_res(res) {
	m_shader = new Shader("./shaders/color.vert", "./shaders/color.frag", ShaderLoadType::FromFile);

	// Provide texture samplers
	i32 samplers[MAX_TEXTURE_SAMPLES];
	for (i32 i = 0; i < MAX_TEXTURE_SAMPLES; i++)
		samplers[i] = i;

	// Providing samplers to the shaders
	m_shader->bind();
	m_shader->set_arrayi("textures", samplers, MAX_TEXTURE_SAMPLES);

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

SnowSystem::~SnowSystem() {
	delete m_shader;
	delete m_camera;
}

void SnowSystem::add_spawn_point(const glm::vec2& point) {
	m_spawn_points.push_back(point);
}

void SnowSystem::spawn() {
	if (m_spawn_timeout > 0.0f) return;

	for (auto spawn_point: m_spawn_points) {
		Snow snow = {
			.pos = { spawn_point.x, spawn_point.y + rand_range(-50, 30) },
			.speed = NORMAL_SPEED,
			.lifetime = static_cast<f32>(rand_range(30, 50)),
		};

		if (m_storm) {
			snow.speed = STORM_SPEED;
			snow.lifetime = 15.0f;
		}

		m_particals.push_back(snow);
	}

	m_spawn_timeout = SPAWN_TIMEOUT;
}

void SnowSystem::update(Renderer* renderer, f64 dt) {
	// Updating camera
	m_camera->recalculate_view_proj();
	glm::mat4 view_proj = m_camera->get_view_proj();

	// Calculate spawn timeout
	if (m_spawn_timeout > 0.0f) {
		if (m_storm)
			m_spawn_timeout -= STORM_SPAWN_DECAY * dt;
		else
			m_spawn_timeout -= SPAWN_DECAY * dt;
		if (m_spawn_timeout <= 0.0f) m_spawn_timeout = 0.0f;
	}

	// Updating particals
	for (auto& snow: m_particals) {
		snow.pos.y -= snow.speed * dt;

		if (snow.dir_change == 0.0f) {
			snow.dir = rand_range(-1, 1);
			snow.dir_change = SNOW_DIR_TIMEOUT * dt;
		} else {
			snow.dir = 0.0f;
			snow.dir_change -= SNOW_DIR_DECAY * dt;
			if (snow.dir_change <= 0.0f)
				snow.dir_change = 0.0f;
		}

		snow.pos.x += snow.speed * snow.dir * dt;

		snow.lifetime -= SNOW_DECAY * dt;
	}

	renderer->begin_pass({0, 0}, m_res);
	{
		renderer->clear({0.5,0.5,0.5,1});

		m_shader->bind();
		m_shader->set_mat4f("mvp", view_proj, false);

		for (auto& snow: m_particals) {
			renderer->push_quad(Quad {
				{ snow.pos.x, snow.pos.y, 0 },
				snow.size,
				glm::mat4(1),
				nullptr,
				{ 0, 0, 1, 1 },
				{ 1, 1, 1, 0.7 }
			});
		}
	}
	renderer->end_pass();

	// Deleting the particals with 0 lifetime
	m_particals.erase(
		std::remove_if(m_particals.begin(), m_particals.end(),
		[](Snow snow) {
			return snow.lifetime <= 0.0f;
		}),
		m_particals.end()
	);
}
