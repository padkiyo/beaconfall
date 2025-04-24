#pragma once

#include "common.h"

struct CameraSpecs {
	f32 aspect_ratio;
	f32 fov;
	f32 near;
	f32 far;
};

struct Camera {
	glm::vec3 pos;
	glm::vec3 dir, up, right;

	glm::mat4 look_at;
	glm::mat4 mvp;

	CameraSpecs specs;
};

Camera camera_create(glm::vec3 pos, glm::vec3 dir, const CameraSpecs& specs);
glm::mat4 camera_calc_mvp(Camera& cam);
