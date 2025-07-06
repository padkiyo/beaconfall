#include "camera.h"

Camera::Camera(glm::vec3 pos, const OrthoCameraSpecs& specs)
	: m_type(CameraType::Ortho), m_pos(pos), m_specs(specs) {
}

Camera::Camera(glm::vec3 pos, glm::vec3 dir, const PerspCameraSpecs& specs)
	: m_type(CameraType::Persp),
		m_pos(pos),
		m_dir(glm::normalize(dir)),
		m_specs(specs)
{
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::normalize(glm::cross(m_dir, m_up));
}

Camera::~Camera() {}

void Camera::change_pos(glm::vec3 dir, f32 speed) {
	m_pos += dir * speed;
}

void Camera::recalculate_view_proj() {
	switch (m_type) {
		case CameraType::Ortho:
			recalculate_view_proj_ortho();
			break;

		case CameraType::Persp:
			recalculate_view_proj_persp();
			break;
	}
}

void Camera::recalculate_view_proj_ortho() {
	auto& specs = std::get<OrthoCameraSpecs>(m_specs);

	// Projection Matrix
	m_proj = glm::ortho(
		specs.left,
		specs.right,
		specs.bottom,
		specs.top,
		specs.near,
		specs.far
	);

	// Transforming
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_pos);

	// View Calculation
	glm::mat4 view = glm::inverse(transform);

	// View Projection Matrix
	m_view_proj = m_proj * view;
}

void Camera::recalculate_view_proj_persp() {
	auto& specs = std::get<PerspCameraSpecs>(m_specs);

	// Projection matrix
	m_proj = glm::perspective(
		glm::radians(specs.fov),
		specs.aspect_ratio,
		specs.near,
		specs.far
	);

	// Camera rotation matrix (custom camera matrix)
	glm::mat4 camera_mat = glm::mat4(1.0f);
	camera_mat[0] = glm::vec4(m_right, 0.0f);
	camera_mat[1] = glm::vec4(m_up, 0.0f);
	camera_mat[2] = glm::vec4(m_dir, 0.0f);
	camera_mat[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// Translation matrix
	glm::mat4 camera_trans = glm::translate(glm::mat4(1.0f), -m_pos);

	// View matrix (Look-At)
	m_look_at = camera_mat * camera_trans;

	// Final View Projection
	m_view_proj = m_proj * m_look_at;
}
