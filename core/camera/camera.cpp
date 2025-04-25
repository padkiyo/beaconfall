#include "camera.h"

Camera camera_create(glm::vec3 pos, const CameraSpecs& specs) {
	return (Camera) {
		.pos = pos,
		.specs = specs,
	};
}

glm::mat4 camera_calc_mvp(Camera* cam) {
	glm::mat4 proj = glm::ortho(
		cam->specs.left,
		cam->specs.right,
		cam->specs.bottom,
		cam->specs.top,
		cam->specs.near,
		cam->specs.far
	);

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), cam->pos);
	glm::mat4 view = glm::inverse(transform);
	cam->mvp = proj * view;
	return cam->mvp;
}

// This was the code for 3D camera
// If we never need one here is the code

// glm::mat4 camera_calc_mvp(Camera& cam) {
// 	// Projection matrix
// 	glm::mat4 proj = glm::perspective(
// 		glm::radians(cam.specs.fov),
// 		cam.specs.aspect_ratio,
// 		cam.specs.near,
// 		cam.specs.far
// 	);
//
// 	// Camera rotation matrix (custom camera matrix)
// 	glm::mat4 camera_mat = glm::mat4(1.0f);
// 	camera_mat[0] = glm::vec4(cam.right, 0.0f);
// 	camera_mat[1] = glm::vec4(cam.up, 0.0f);
// 	camera_mat[2] = glm::vec4(cam.dir, 0.0f);
// 	camera_mat[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
//
// 	// Translation matrix
// 	glm::mat4 camera_trans = glm::translate(glm::mat4(1.0f), -cam.pos);
//
// 	// View matrix (Look-At)
// 	cam.look_at = camera_mat * camera_trans;
//
// 	// Final MVP
// 	cam.mvp = proj * cam.look_at;
// 	return cam.mvp;
// }
