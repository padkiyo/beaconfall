#pragma once

#include "common.h"


/*
 * Types of Camera that we support
 */

enum class CameraType {
	Ortho,
	Persp,
};


/*
 * Specifications for Orthographic Camera
 */

struct OrthoCameraSpecs {
	f32 left;
	f32 right;
	f32 top;
	f32 bottom;
	f32 near;
	f32 far;
};


/*
 * Specifications for Perspective Camera
 */

struct PerspCameraSpecs {
	f32 fov;
	f32 aspect_ratio;
	f32 near;
	f32 far;
};


/*
 * Camera Class that can handle both types of projections
 * Can calculate view projection matrix based on the type of camera we're using
 */

class Camera {
public:
	Camera(glm::vec3 pos, const OrthoCameraSpecs& specs);
	Camera(glm::vec3 pos, glm::vec3 dir, const PerspCameraSpecs& specs);
	~Camera();

	// dir = Unit Moving Direction (eg: left { -1, 0, 0 }, right { 1, 0, 0 })
	void change_pos(glm::vec3 dir, f32 speed);
	void recalculate_view_proj();

	//---------
	// Getters
	//---------

	inline glm::vec3 get_pos() const { return m_pos; }
	inline glm::mat4 get_proj() const { return m_proj; }
	inline glm::mat4 get_view_proj() const { return m_view_proj; }
	inline glm::mat4 get_look_at() const {
		panic(m_type == CameraType::Persp, "Cannot take a look_at matrix from an Orthographic Camera");
		return m_look_at;
	}

private:
	void recalculate_view_proj_ortho();
	void recalculate_view_proj_persp();

private:
	glm::vec3 m_pos;
	glm::mat4 m_look_at, m_proj, m_view_proj;

	// Perspective types
	glm::vec3 m_dir, m_up, m_right;

	std::variant<OrthoCameraSpecs, PerspCameraSpecs> m_specs;
	CameraType m_type;
};
