#include "camera.h"

Camera camera_create(glm::vec3 pos, glm::vec3 dir, const CameraSpecs& specs) {
	dir = glm::normalize(dir);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(dir, up));

	return (Camera) {
		.pos = pos,
		.dir = dir,
		.up = up,
		.right = right,
		.specs = specs,
	};
}

glm::mat4 camera_calc_mvp(Camera& cam) {
	// Projection matrix
	glm::mat4 proj = glm::perspective(
		glm::radians(cam.specs.fov),
		cam.specs.aspect_ratio,
		cam.specs.near,
		cam.specs.far
	);

	// Camera rotation matrix (custom camera matrix)
	glm::mat4 camera_mat = glm::mat4(1.0f);
	camera_mat[0] = glm::vec4(cam.right, 0.0f);
	camera_mat[1] = glm::vec4(cam.up, 0.0f);
	camera_mat[2] = glm::vec4(cam.dir, 0.0f);
	camera_mat[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// Translation matrix
	glm::mat4 camera_trans = glm::translate(glm::mat4(1.0f), -cam.pos);

	// View matrix (Look-At)
	cam.look_at = camera_mat * camera_trans;

	// Final MVP
	cam.mvp = proj * cam.look_at;
	return cam.mvp;
}
