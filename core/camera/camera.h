#pragma once

#include "common.h"

struct CameraSpecs {
	f32 left;
	f32 right;
	f32 top;
	f32 bottom;
	f32 near;
	f32 far;
};

struct Camera {
	glm::vec3 pos;
	glm::mat4 mvp;
	CameraSpecs specs;
};

Camera* camera_create(glm::vec3 pos, const CameraSpecs& specs);
void camera_destroy(Camera* cam);
glm::mat4 camera_calc_mvp(Camera* cam);
